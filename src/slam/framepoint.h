#pragma once

#include "src/common/image.h"

namespace slam {

class FeatureFrame;
class WorldPoint;

class PointBase
{
protected:
    PointBase();
};

class MonoPoint : public PointBase
{
public:
    using AdjacentPtr = std::shared_ptr< MonoPoint >;
    using WorldPointPtr = std::shared_ptr< WorldPoint >;

    virtual const cv::Point2f &point() const = 0;

    void setStereoPoint( const AdjacentPtr point );
    AdjacentPtr stereoPoint() const;

    void setNextPoint( const AdjacentPtr point );
    AdjacentPtr nextPoint() const;

    void setPrevPoint( const AdjacentPtr point );
    AdjacentPtr prevPoint() const;

    void setWorldPoint( const WorldPointPtr point );
    WorldPointPtr worldPoint() const;

     void drawTrack( CvImage *target ) const;

protected:
    using AdjacentPtrImpl = std::weak_ptr< MonoPoint >;
    using WorldPointPtrImpl = std::weak_ptr< WorldPoint >;

    MonoPoint();

    AdjacentPtrImpl m_stereoPoint;
    AdjacentPtrImpl m_nextPoint;
    AdjacentPtrImpl m_prevPoint;

    WorldPointPtrImpl m_worldPoint;

};

class FeaturePoint : public MonoPoint
{
public:
    using FramePtr = std::weak_ptr< FeatureFrame >;
    using PointPtr = std::shared_ptr< FeaturePoint >;

    virtual const cv::Point2f &point() const override;

    static PointPtr create( const FramePtr parentFrame, const size_t keyPointIndex );

protected:
    FeaturePoint( const FramePtr parentFrame, const size_t keyPointIndex );

    const FramePtr m_parentFrame; // Parent frame
    size_t m_keyPointIndex; // Index of keypoint in parent frame

private:

};

class Point : public MonoPoint
{
};

class DoublePoint : public PointBase
{
public:
    using MonoPointPtr = std::shared_ptr< MonoPoint >;

    void setMonoPoints( const MonoPointPtr point1, const MonoPointPtr point2 );

    MonoPointPtr monoPoint1() const;
    MonoPointPtr monoPoint2() const;

protected:
    DoublePoint( const MonoPointPtr point1, const MonoPointPtr point2 );

    MonoPointPtr m_point1;
    MonoPointPtr m_point2;

};

}
