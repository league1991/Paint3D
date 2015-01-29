#include "StdAfx.h"
#include "BlendCurveGraphWidget.h"


BlendCurveGraphWidget::BlendCurveGraphWidget( QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/ ): QWidget(parent, flags)
{
	for (int i = 0; i < BLEND_CURVE_WIDGET_SEGMENTS;++i)
	{
		valueArray[i] = 1.0f;
	}
}

BlendCurveGraphWidget::~BlendCurveGraphWidget(void)
{
}


void BlendCurveGraphWidget::paintEvent( QPaintEvent *event )
{
	QPainter painter(this);
	QSize size = this->size();
	painter.setBrush(QBrush(QColor(150,150,150,255)));
	painter.setPen(Qt::NoPen);
	QVector<QPointF> points;
	for (int i = 0; i < BLEND_CURVE_WIDGET_SEGMENTS; ++i)
	{
		float x = i / float(BLEND_CURVE_WIDGET_SEGMENTS - 1) * size.width() ;
		float y = (1.0f - valueArray[i]) * size.height();
		points.push_back(QPointF(x,y));
	}
	points.push_back(QPointF(size.width(), size.height()));
	points.push_back(QPointF(0, size.height()));
	painter.drawPolygon(&points[0], points.size());
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::SolidLine);
	painter.drawRect(0,0,size.width()-1, size.height()-1);
}


void BlendCurveGraphWidget::setCurve( float v0, float v1, float w0, float w1 )
{
	Brush::computeBlendCurve(valueArray, BLEND_CURVE_WIDGET_SEGMENTS, v0, v1, w0, w1);
	this->update();
}
