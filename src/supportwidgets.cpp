#include "precompiled.h"

#include "supportwidgets.h"
#include "defs.h"

// TypeComboBox
TypeComboBox::TypeComboBox(QWidget *parent )
    : QComboBox( parent )
{
    initialize();
}

void TypeComboBox::initialize()
{
    addItem( tr( "Checkerboard" ), TemplateProcessor::CHECKERBOARD );
    addItem( tr( "Circles" ), TemplateProcessor::CIRCLES );
    addItem( tr( "Asymetric circles" ), TemplateProcessor::ASYM_CIRCLES );
}

TemplateProcessor::Type TypeComboBox::currentType() const
{
    return static_cast<TemplateProcessor::Type>( currentData().toInt() );
}

// CountSpinBox
CountSpinBox::CountSpinBox( QWidget *parent )
    : QSpinBox( parent )
{
    initialize();
}

void CountSpinBox::initialize()
{
    setMinimum( 3 );
    setMaximum( 100 );

    setValue( 5 );

    setAlignment( Qt::AlignRight );

}

// SizeSpinBox
SizeSpinBox::SizeSpinBox( QWidget *parent )
    : QDoubleSpinBox( parent )
{
    initialize();
}

void SizeSpinBox::initialize()
{
    setMinimum( 0 );
    setMaximum( 1e6 );

    setValue( 1.0 );

    setAlignment( Qt::AlignRight );

}

// RescaleSpinBox
RescaleSpinBox::RescaleSpinBox( QWidget *parent )
    : QSpinBox( parent )
{
    initialize();
}

void RescaleSpinBox::initialize()
{
    setMinimum( 300 );
    setMaximum( 4000 );

    setValue( 800 );

    setAlignment( Qt::AlignRight );

}

// SliderBoxBase
SliderBoxBase::SliderBoxBase( const QString label, QWidget* parent )
    : QWidget( parent )
{
    initialize( label );
}

void SliderBoxBase::initialize( const QString label )
{
    m_layout = new QHBoxLayout( this );

    m_label = new QLabel( label, this );
    m_slider = new QSlider( Qt::Horizontal, this );

    m_layout->addWidget( m_label );
    m_layout->addWidget( m_slider );

}

// IntSliderBox
IntSliderBox::IntSliderBox( const QString label, QWidget* parent )
    : SliderBoxBase( label, parent )
{
    initialize();
}

void IntSliderBox::initialize()
{
    m_spinBox = new QSpinBox( this );

    m_layout->addWidget( m_spinBox );

    connect( m_slider, &QSlider::sliderMoved, m_spinBox,  &QSpinBox::setValue );
    connect( m_spinBox,  static_cast< void ( QSpinBox::* )( int ) >( &QSpinBox::valueChanged ), m_slider, &QSlider::setValue );

}

int IntSliderBox::value()
{
    return m_spinBox->value();
}

void IntSliderBox::setMinimum( const int value )
{
    m_slider->setMinimum( value );
    m_spinBox->setMinimum( value );
}

void IntSliderBox::setMaximum( const int value )
{
    m_slider->setMaximum( value );
    m_spinBox->setMaximum( value );
}

void IntSliderBox::setStepSize( const int value )
{
    m_slider->setSingleStep( value );
    m_spinBox->setSingleStep( value );
}

void IntSliderBox::setValue( const int value )
{
    m_spinBox->setValue( value );
}

// DoubleSliderBox
DoubleSliderBox::DoubleSliderBox( const QString label, QWidget* parent )
    : SliderBoxBase( label, parent )
{
    initialize();
}

void DoubleSliderBox::initialize()
{
    m_spinBox = new QDoubleSpinBox( this );

    m_layout->addWidget( m_spinBox );

    connect( m_slider, &QSlider::sliderMoved, [&]( int position)  {
        double value = static_cast< double >( position ) / 100.0;
        if ( fabs(m_spinBox->value() - value) > DOUBLE_EPS ) {
            m_spinBox->setValue( value );
        }
    } );
    connect( m_spinBox,  static_cast< void ( QDoubleSpinBox::* )( double ) >( &QDoubleSpinBox::valueChanged ), [&]( double position ) {
        int value = position * 100.0;
        if ( m_slider->value() != value ) {
            m_slider->setValue( value );
        }
    } );

}

double DoubleSliderBox::value()
{
    return m_spinBox->value();
}

void DoubleSliderBox::setMinimum( const double value )
{
    m_slider->setMinimum( value * 100 );
    m_spinBox->setMinimum( value );
}

void DoubleSliderBox::setMaximum( const double value )
{
    m_slider->setMaximum( value * 100 );
    m_spinBox->setMaximum( value );
}

void DoubleSliderBox::setStepSize( const double value )
{
    m_slider->setSingleStep( value * 100 );
    m_spinBox->setSingleStep( value );
}

void DoubleSliderBox::setValue(const double value )
{
    m_spinBox->setValue( value );
}

