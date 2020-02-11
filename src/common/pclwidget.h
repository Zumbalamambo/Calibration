#pragma once

#include "colorpoint.h"

#include <QVTKOpenGLWidget.h>
#include <vtkRenderWindow.h>
#include <pcl/visualization/pcl_visualizer.h>

class PCLWidget : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    PCLWidget( QWidget* parent = nullptr );

    void setPointCloud( const pcl::PointCloud< pcl::PointXYZRGB >::Ptr cloud, const std::string &id = "cloud" );
    void setPointCloud( const std::list< ColorPoint3d > &points, const std::string &id = "cloud" );

    bool contains( const std::string &id ) const;

public slots:
    void update();

protected:
    vtkSmartPointer< vtkRenderer > m_renderer;

    pcl::visualization::PCLVisualizer::Ptr m_pclViewer;

private:
    void initialize();

};
