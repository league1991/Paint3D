#pragma once
#include "QWidget"
#include "ui_brushSetting.h"

class BrushSettingWidget: public QScrollArea, Ui::BrushSetting
{
	Q_OBJECT
public:
	BrushSettingWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BrushSettingWidget(void);

private slots:
	void setSize(double size);
	void setResolutionRatio(double ratio);
	void setAngle(double angle);
	void setRatio(double ratio);
	void setSizeJitter(double sizeJitter);
	void setAngleJitter(double angleJitter);
	void setRatioJitter(double ratioJitter);
	void setOffsetJitter(double offsetJitter);
	void setInterval(double interval);
	void setHardness(double hard);
	void setDepthRange(double dummy);


	void loadStrokeFromImage();
	void loadStrokeFromAbr();
	void setStroke(int ithStroke);
private:
	void refreshListViewItem();
};
