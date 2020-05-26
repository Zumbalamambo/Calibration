#include "src/common/precompiled.h"

#include "map.h"

#include "mappoint.h"

#include "frame.h"

#include "world.h"

#include "src/common/defs.h"
#include "src/common/functions.h"

namespace slam {

const double FeatureMap::m_minTriangulateDistanceMultiplier = 2.0;

const double FeatureMap::m_minTrackInliersRatio = 0.75;
const double FeatureMap::m_goodTrackInliersRatio = 0.9;

FeatureMap::FeatureMap( const StereoCameraMatrix &projectionMatrix, const WorldPtr &parentWorld)
    :  m_parentWorld( parentWorld )
{
    initialize( projectionMatrix );
}

void FeatureMap::initialize( const StereoCameraMatrix &projectionMatrix )
{
    m_projectionMatrix = projectionMatrix;

    m_previousKeypointsCount = m_goodTrackPoints * 2;
}

FeatureMap::MapPtr FeatureMap::create( const StereoCameraMatrix &cameraMatrix , const WorldPtr &parentWorld )
{
    return MapPtr( new FeatureMap( cameraMatrix, parentWorld ) );
}

FeatureMap::WorldPtr FeatureMap::parentWorld() const
{
    return m_parentWorld.lock();
}

FeatureMap::MapPointPtr FeatureMap::createMapPoint( const cv::Point3d &pt, const cv::Scalar &color )
{
    auto point = MapPoint::create( shared_from_this(), pt, color );

    addMapPoint( point );

    return point;

}

void FeatureMap::removeMapPoint( const MapPointPtr &point )
{
    m_mapPoints.erase( point );
}

std::list< FeatureMap::FramePtr > FeatureMap::frames() const
{
    m_mutex.lock();
    auto ret = m_frames;
    m_mutex.unlock();

    return ret;
}

std::set< FeatureMap::MapPointPtr > FeatureMap::mapPoints() const
{
    m_mutex.lock();
    auto ret = m_mapPoints;
    m_mutex.unlock();

    return ret;
}

const FeatureMap::FramePtr &FeatureMap::backFrame() const
{
    return m_frames.back();
}

void FeatureMap::addMapPoint( const MapPointPtr &point )
{
    m_mapPoints.insert( point );
}

const cv::Mat FeatureMap::baselineVector() const
{
    return m_projectionMatrix.baselineVector();
}

double FeatureMap::baselineLenght() const
{
    return cv::norm( baselineVector() );
}

double FeatureMap::minTriangulateCameraDistance() const
{
    return baselineLenght() * m_minTriangulateDistanceMultiplier;
}

void FeatureMap::adjust( const int frames )
{
    const std::lock_guard< std::mutex > lock( m_mutex );

    std::list< FramePtr > list;

    int counter = 0;

    for ( auto i = m_frames.rbegin(); i != m_frames.rend() && counter < frames; ++i, ++counter )
        if ( std::dynamic_pointer_cast< StereoFrame >( *i ) )
            list.push_front( *i );

    m_optimizer.adjust( list );

}

size_t calcTrackLenght( const std::vector< std::shared_ptr< MonoPoint > > &points )
{
    size_t ret = 0;

    for ( auto &i : points )
        if ( i )
            ret = std::max( ret, i->prevTrackLenght() );

    return ret;

}

void FeatureMap::localAdjustment()
{
    const std::lock_guard< std::mutex > lock( m_mutex );

    if ( !m_frames.empty() ) {

        std::list< FramePtr > list;

        auto backFrame = m_frames.back();

        if ( backFrame ) {

            auto leftFrame = backFrame->leftFrame();
            auto rightFrame = backFrame->rightFrame();

            if ( leftFrame && rightFrame ) {

                auto count = std::max( calcTrackLenght( leftFrame->framePoints() ), calcTrackLenght( rightFrame->framePoints() ) );

                size_t counter = 0;

                for ( auto i = m_frames.rbegin(); i != m_frames.rend() && counter < count; ++i, ++counter )
                    list.push_front( *i );

                m_optimizer.adjust( list );

            }

        }

    }

}

bool FeatureMap::isRudimental() const
{
    return m_frames.size() <= 1;
}

bool FeatureMap::track( const CvImage &leftImage, const CvImage &rightImage )
{
    auto denseFrame = FeatureDenseFrame::create( shared_from_this() );

    denseFrame->load( leftImage, rightImage );

    /*if ( m_frames.size() % 5 == 0 )
        denseFrame->processDenseCloud();*/

    const std::lock_guard< std::mutex > lock( m_mutex );

    if ( m_frames.empty() ) {

        denseFrame->setProjectionMatrix( m_projectionMatrix );
        m_frames.push_back( denseFrame );

    }
    else {
        auto previousDenseFrame = std::dynamic_pointer_cast< FeatureDenseFrame >( m_frames.back() );

        if ( previousDenseFrame ) {

            auto previousLeftFrame = std::dynamic_pointer_cast< FeatureFrame >( previousDenseFrame->leftFrame() );
            auto previousRightFrame = std::dynamic_pointer_cast< FeatureFrame >( previousDenseFrame->rightFrame() );
            auto leftFrame = std::dynamic_pointer_cast< FeatureFrame >( denseFrame->leftFrame() );
            auto rightFrame = std::dynamic_pointer_cast< FeatureFrame >( denseFrame->rightFrame() );

            auto adjacentLeftFrame = FeatureConsecutiveFrame::create( shared_from_this() );

            adjacentLeftFrame->setFrames( previousLeftFrame, leftFrame );

            auto keypointsCount = m_previousKeypointsCount;

            previousLeftFrame->triangulatePoints();

            int trackedPointCount;

            double inliersRatio = 0.0;

            auto maxKeypointsCount = previousLeftFrame->extractedPointsCount();

            int triangulatePointsCount = 0;

            do {

                do {

                    previousLeftFrame->createFramePoints( keypointsCount );

                    previousDenseFrame->match();
                    triangulatePointsCount = previousDenseFrame->triangulatePoints();

                    auto trackFramePointsCount = std::max( 0, m_trackFramePointsCount - adjacentLeftFrame->trackFramePointsCount() );

                    adjacentLeftFrame->createFramePoints( trackFramePointsCount );

                    adjacentLeftFrame->track();

                    trackedPointCount = adjacentLeftFrame->posePointsCount();

                    m_previousKeypointsCount = keypointsCount;

                    keypointsCount *= 2;

                } while( trackedPointCount < m_goodTrackPoints && m_previousKeypointsCount <= maxKeypointsCount );

                inliersRatio = adjacentLeftFrame->recoverPose();

            } while ( inliersRatio < m_goodTrackInliersRatio && m_previousKeypointsCount <= maxKeypointsCount );

            std::cout << "Prev keypoints count: " << previousLeftFrame->extractedPointsCount() << std::endl;
            std::cout << "Cur keypoints count: " << leftFrame->extractedPointsCount() << std::endl;
            std::cout << "Stereo points count: " << previousDenseFrame->stereoPointsCount() << std::endl;
            std::cout << "Triangulated points count: " << triangulatePointsCount << std::endl;
            std::cout << "Adjacent points count: " << adjacentLeftFrame->adjacentPointsCount() << std::endl;
            std::cout << "Tracked points count: " << trackedPointCount << std::endl;

            std::cout << "Inliers: " << inliersRatio << std::endl;

            if ( trackedPointCount < m_minTrackPoints || inliersRatio < m_minTrackInliersRatio )
                return false;

            if ( trackedPointCount > m_overTrackPoints )
                m_previousKeypointsCount /= 2;

            rightFrame->setCameraMatrix( previousRightFrame->cameraMatrix() );
            rightFrame->setRotation( leftFrame->rotation() );
            rightFrame->setTranslation( leftFrame->translation() + baselineVector() );

            previousDenseFrame->cleanMapPoints();

            auto replacedFrame = DenseFrame::create( shared_from_this() );
            replacedFrame->replaceAndClean( previousDenseFrame );

            m_frames.back() = replacedFrame;

        }

        m_frames.push_back( denseFrame );

    }

    return true;

}

}

