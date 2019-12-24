#pragma once

#include "src/common/calibrationdatabase.h"
#include "src/common/rectificationprocessor.h"

#include "map.h"

namespace slam {

class World : public std::enable_shared_from_this< World >
{
public:

    using WorldPtr = std::shared_ptr< World >;

    using FramePtr = Map::FramePtr;

    using MapPtr = Map::MapPtr;
    using MapPointPtr = Map::MapPointPtr;

    static WorldPtr create( const ProjectionMatrix &leftProjectionMatrix, const ProjectionMatrix &rightProjectionMatrix );

    const std::list < MapPtr > &maps() const;
    std::list < MapPtr > &maps();

    bool track( const CvImage &leftImage, const CvImage &rightImage );

    void adjust( const int frames );

    void multiplicateCameraMatrix( const double value );
    void movePrincipalPoint( const cv::Vec2f &value );

protected:
    World( const ProjectionMatrix &leftProjectionMatrix, const ProjectionMatrix &rightProjectionMatrix );

    std::list < MapPtr > m_maps;

    static const int m_keypointsCount = 25000;

private:
    void initialize();

};

}
