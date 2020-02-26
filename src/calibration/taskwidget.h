#pragma once

#include <QWidget>
#include <QPointer>

#include "camerawidget.h"

class QVBoxLayout;

class CameraParametersWidget;

class GrabWidgetBase : public QWidget
{
    Q_OBJECT

public:
    explicit GrabWidgetBase( QWidget* parent = nullptr );

    TypeComboBox::Type templateType() const;
    const cv::Size templateCount() const;
    double templateSize() const;
    double intervalSize() const;
    bool resizeFlag() const;
    unsigned int frameMaximumSize() const;

    bool adaptiveThreshold() const;
    bool normalizeImage() const;
    bool filterQuads() const;
    bool fastCheck() const;

protected slots:
    void updateParameters();

protected:
    QPointer<QVBoxLayout> m_layout;
    QPointer< CameraParametersWidget > m_parametersWidget;
    QPointer< CameraWidgetBase > m_cameraWidget;

private:
    void initialize();

};

class MonocularGrabWidget : public GrabWidgetBase
{
    Q_OBJECT

public:
    explicit MonocularGrabWidget( const QString &cameraIp, QWidget* parent = nullptr );

    MonocularCameraWidget *cameraWidget() const;

    const CvImage sourceImage() const;
    const CvImage previewImage() const;

    bool isTemplateExist() const;

private:
    void initialize (const QString &cameraIp );

};


class StereoGrabWidget : public GrabWidgetBase
{
    Q_OBJECT

public:
    explicit StereoGrabWidget( const QString &leftCameraIp, const QString &rightCameraIp, QWidget* parent = nullptr );

    StereoCameraWidget *cameraWidget() const;

    const CvImage leftDisplayedImage() const;
    const CvImage rightDisplayedImage() const;

    bool isTemplateExist() const;

    const StereoFrame frame() const;

public slots:
    void setLeftSourceImage( const CvImage image );
    void setLeftDisplayedImage( const CvImage image );

    void setRightSourceImage( const CvImage image );
    void setRightDisplayedImage( const CvImage image );

private:
    void initialize( const QString &leftCameraIp, const QString &rightCameraIp );

};


