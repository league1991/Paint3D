#pragma once
#include "ui_paintSetting.h"

class Paint3DFrame;

class PaintSettingWidget:public QScrollArea, public Ui::PaintSetting
{
	Q_OBJECT
public:
	PaintSettingWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~PaintSettingWidget(void);

public slots:
	void updateWidgets();
	void setColor( QColor color );
	void setColor( double dummy );
	void setSurface(double dummy);
	void setThickness(double thick);
	void setPickerOpacity(double opacity);
	void attachToCamera(bool isAttach);
	void setAttachingParam(double dummy);

	void setColorTex();
	void setReflTex();
	void setGlosTex();
	void setRefrTex();
	void setIORTex();
	void setThicknessTex();

	void clearColorTex();
	void clearReflTex();
	void clearGlosTex();
	void clearRefrTex();
	void clearIORTex();
	void clearThicknessTex();

	void setAffectColor(int isAffect);
	void setAffectReflLevel(int isAffect);
	void setAffectRefrLevel(int isAffect);
	void setAffectGlossness(int isAffect);
	void setAffectIOR(int isAffect);
	void setAffectThickness(int isAffect);

	void computeNormalBlendCurve(double dummy);
	void computeDepthBlendCurve(double dummy);
	void setDepthMode(int mode);
	void changeColorPicker(bool dummy);
	void pickColorPickerFromScene();
	void refreshPickerList();

	void setBrushMode(bool isBrushMode);

private:
	QString openImageFile();
	QImage convertToGrayScale(const QImage& img);
	void detachFromCamera();
	QPixmap nullPixmap;
};

void getPickedObject(QSharedPointer<RenderableObject> obj);