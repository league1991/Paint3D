#ifndef SLIDERSPINNERGROUP_H
#define SLIDERSPINNERGROUP_H

#include <QtGui/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>

#define SLIDER_MIN_VALUE 0
#define SLIDER_MAX_VALUE 1000

class SliderSpinnerGroup : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)

public:
    SliderSpinnerGroup(QWidget *parent = 0);


signals:
    void valueChanged(double val);

private slots:
    void setSliderValue(double val);
    void setSpinBoxValue(int val);

public slots:
    double value(){return m_spinBox->value();}
    double minimum(){return m_spinBox->minimum();}
    double maximum(){return m_spinBox->maximum();}
    double singleStep(){return m_spinBox->singleStep();}

    void setValue(double val)
    {
        m_spinBox->setValue(val);
    }
    void setMinimum(double val)
    {
        m_spinBox->setMinimum(val);
        setSliderValue(m_spinBox->value());
    }
    void setMaximum(double val)
    {
        m_spinBox->setMaximum(val);
        setSliderValue(m_spinBox->value());
    }
    void setSingleStep(double val)
    {
        m_spinBox->setSingleStep(val);
        setSliderValue(m_spinBox->value());
    }


private:
    QSlider* m_slider;
    QDoubleSpinBox* m_spinBox;
    QHBoxLayout* layout;
};

#endif
