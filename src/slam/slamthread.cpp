#include "src/common/precompiled.h"

#include "slamthread.h"

#include "src/common/rectificationprocessor.h"

#include "world.h"
#include "mappoint.h"
#include "frame.h"

#include <thread>

// SlamThread
SlamThread::SlamThread( QObject *parent )
    : QThread( parent )
{
    initialize();
}

void SlamThread::initialize()
{
    m_path = std::string ("/home/victor/Polygon/");

    m_scaleFactor = 1.0;

    StereoCalibrationDataShort calibration( m_path + "calibration.yaml" );

    auto cropRect = calibration.cropRect();
    auto principal = cv::Vec2f( -cropRect.x, -cropRect.y );

    auto projectionMatrix = calibration.projectionMatrix();

    projectionMatrix.movePrincipalPoint( principal );
    projectionMatrix.multiplicateCameraMatrix( m_scaleFactor );

    m_system = slam::World::create( projectionMatrix );



}

void SlamThread::run()
{
    auto localOptimizationThread = std::thread( [ & ] {

        while ( true ) {

            m_system->localAdjustment();
            std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

        }

    } );

    localOptimizationThread.detach();

    auto optimizationThread5s = std::thread( [ & ] {

        while ( true ) {

            const int frames = 100;

            m_system->adjust( frames );
            std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

        }

    } );

    optimizationThread5s.detach();

    auto leftPath = m_path + "left/";
    auto rightPath = m_path + "right/";

    StereoCalibrationDataShort calibration( m_path + "calibration.yaml" );
    StereoRectificationProcessor rectificationProcessor( calibration );

    for ( auto i = 5900; i < 30000; ++i ) {

        std::cout << i << std::endl;

        std::string leftFile = leftPath + std::to_string( i ) + "_left.jpg";
        std::string rightFile = rightPath + std::to_string( i ) + "_right.jpg";

        CvImage leftImage( leftFile );
        CvImage rightImage( rightFile );

        CvImage leftRectifiedImage;
        CvImage rightRectifiedImage;
        CvImage leftCroppedImage;
        CvImage rightCroppedImage;

        if ( rectificationProcessor.rectify( leftImage, rightImage, &leftRectifiedImage, &rightRectifiedImage )
                    && rectificationProcessor.crop( leftRectifiedImage, rightRectifiedImage, &leftCroppedImage, &rightCroppedImage ) ) {

            CvImage leftResizedImage;
            CvImage rightResizedImage;

            cv::resize( leftCroppedImage, leftResizedImage, cv::Size(), m_scaleFactor, m_scaleFactor, cv::INTER_CUBIC );
            cv::resize( rightCroppedImage, rightResizedImage, cv::Size(), m_scaleFactor, m_scaleFactor, cv::INTER_CUBIC );

            auto time = std::chrono::system_clock::now();

            m_system->track( leftResizedImage, rightResizedImage );

            std::cout << std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::system_clock::now() - time ).count()  / 1.e6 << " sec" << std::endl;

            emit updateSignal();

        }

    }

}

const std::list < SlamThread::MapPtr > &SlamThread::maps() const
{
    return m_system->maps();
}

CvImage SlamThread::pointsImage() const
{
    if ( !m_system->maps().empty() ) {

        if ( !m_system->maps().back()->frames().empty() ) {

            auto processedFrame = std::dynamic_pointer_cast< slam::ProcessedStereoFrame >( m_system->maps().back()->frames().back() );

            if ( processedFrame )
                return processedFrame->drawPoints();

        }

    }

    return CvImage();

}

CvImage SlamThread::tracksImage() const
{
    if ( !m_system->maps().empty() ) {

        if ( !m_system->maps().back()->frames().empty() ) {

            auto processedFrame = std::dynamic_pointer_cast< slam::ProcessedStereoFrame >( m_system->maps().back()->frames().back() );

            if ( processedFrame )
                return processedFrame->leftFrame()->drawTracks();

        }

    }

    return CvImage();

}

CvImage SlamThread::stereoImage() const
{
    if ( !m_system->maps().empty() ) {

        if ( m_system->maps().back()->frames().size() > 1 ) {

            auto processedFrame = std::dynamic_pointer_cast< slam::ProcessedStereoFrame >( *(++m_system->maps().back()->frames().rbegin()) );

            if ( processedFrame )
                return processedFrame->drawStereoCorrespondences();

        }

    }

    return CvImage();

}
