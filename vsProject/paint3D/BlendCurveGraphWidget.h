#pragma once
#define  BLEND_CURVE_WIDGET_SEGMENTS 30 
class BlendCurveGraphWidget :
	public QWidget
{
public:
	BlendCurveGraphWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BlendCurveGraphWidget(void);

	void setCurve(float v0, float v1, float w0, float w1);

protected:   
	void paintEvent(QPaintEvent *event);
private:
	float valueArray[BLEND_CURVE_WIDGET_SEGMENTS];
};
