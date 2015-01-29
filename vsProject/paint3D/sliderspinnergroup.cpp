#include "stdafx.h"
#include "sliderspinnergroup.h"

SliderSpinnerGroup::SliderSpinnerGroup(QWidget *parent) :
    QWidget(parent)
{
    m_slider = new QSlider(Qt::Horizontal,this);
    m_slider->setRange(SLIDER_MIN_VALUE, SLIDER_MAX_VALUE);
    m_slider->setFocusPolicy(Qt::StrongFocus);
    m_slider->setTickPosition(QSlider::NoTicks);

    m_spinBox = new QDoubleSpinBox(this);
    m_spinBox->setRange(0,1);
    m_spinBox->setSingleStep(0.05);
    m_spinBox->setDecimals(2);
    m_spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_spinBox->setMaximumWidth(60);

    layout = new QHBoxLayout();
    layout->addWidget(m_spinBox);
    layout->addWidget(m_slider);
    layout->setContentsMargins(0,0,0,0);

    this->setLayout(layout);

    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(setSpinBoxValue(int)));
    connect(m_spinBox, SIGNAL(valueChanged(double)), this, SLOT(setSliderValue(double)));

    connect(m_spinBox, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));

}


void SliderSpinnerGroup::setSliderValue(double val)
{
    float delta = m_spinBox->maximum() - m_spinBox->minimum();
    int   sliderVal = (val - m_spinBox->minimum()) / delta * (SLIDER_MAX_VALUE - SLIDER_MIN_VALUE) + 0.5;
    float ratio = (sliderVal - SLIDER_MIN_VALUE) / float(SLIDER_MAX_VALUE - SLIDER_MIN_VALUE);
    float spinBoxValue = m_spinBox->minimum() + delta * ratio;
    // 首先对原来spinBox的值修正，设定为slider实际对应的值（因为slider值取整数，会出现误差）
    // 这样就可以避免出现无法通过spinBox调整slider的情况
    m_spinBox->setValue(spinBoxValue);
    // 最后再设置spinBox的值
    m_slider->setValue(sliderVal);
}

void SliderSpinnerGroup::setSpinBoxValue(int val)
{
    float delta = m_spinBox->maximum() - m_spinBox->minimum();
    float ratio = (val - SLIDER_MIN_VALUE) / float(SLIDER_MAX_VALUE - SLIDER_MIN_VALUE);
    float spinBoxValue = m_spinBox->minimum() + delta * ratio;
    m_spinBox->setValue(spinBoxValue);
}
