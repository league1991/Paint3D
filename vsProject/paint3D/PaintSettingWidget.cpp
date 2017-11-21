#include "StdAfx.h"
#include "PaintSettingWidget.h"
#include "Paint3DFrame.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
PaintSettingWidget::PaintSettingWidget( QWidget *parent /*= 0*/, Qt::WindowFlags flags /*= 0*/ )
{
	setupUi(this);

	nullPixmap = QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png"));

	connect(colorButton, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
	connect(opacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(setColor(double)));
	connect(reflLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSurface(double)));
	connect(reflGlossnessSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSurface(double)));
	connect(refrLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSurface(double)));
	connect(refrGlossnessSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSurface(double)));
	connect(thicknessSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setThickness(double)));
	connect(pickerOpacitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPickerOpacity(double)));

	connect(xOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));
	connect(yOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));
	connect(zOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));
	connect(rotateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAttachingParam(double)));

	connect(colorTexButton, SIGNAL(clicked()), this, SLOT(setColorTex()));
	connect(reflTexButton, SIGNAL(clicked()), this, SLOT(setReflTex()));
	connect(glossnessTexButton, SIGNAL(clicked()), this, SLOT(setGlosTex()));
	connect(refrTexButton, SIGNAL(clicked()), this, SLOT(setRefrTex()));
	connect(iorTexButton, SIGNAL(clicked()), this, SLOT(setIORTex()));
	connect(thicknessTexButton, SIGNAL(clicked()), this, SLOT(setThicknessTex()));

	connect(clearColorTexButton, SIGNAL(clicked()), this, SLOT(clearColorTex()));
	connect(clearReflTexButton, SIGNAL(clicked()), this, SLOT(clearReflTex()));
	connect(clearGlossnessTexButton, SIGNAL(clicked()), this, SLOT(clearGlosTex()));
	connect(clearRefrTexButton, SIGNAL(clicked()), this, SLOT(clearRefrTex()));
	connect(clearIorTexButton, SIGNAL(clicked()), this, SLOT(clearIORTex()));
	connect(clearThicknessTexButton, SIGNAL(clicked()), this, SLOT(clearThicknessTex()));

	connect(attachToCameraBox, SIGNAL(toggled(bool)), this, SLOT(attachToCamera(bool)));

	connect(normalV0Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeNormalBlendCurve(double)));
	connect(normalV1Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeNormalBlendCurve(double)));
	connect(normalW0Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeNormalBlendCurve(double)));
	connect(normalW1Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeNormalBlendCurve(double)));

	connect(depthV0Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeDepthBlendCurve(double)));
	connect(depthV1Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeDepthBlendCurve(double)));
	connect(depthW0Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeDepthBlendCurve(double)));
	connect(depthW1Spinner, SIGNAL(valueChanged(double)), this, SLOT(computeDepthBlendCurve(double)));


	connect(depthTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDepthMode(int)));

	connect(affectColor, SIGNAL(stateChanged(int)), this, SLOT(setAffectColor(int)));
	connect(affectReflLevel, SIGNAL(stateChanged(int)), this, SLOT(setAffectReflLevel(int)));
	connect(affectRefrLevel, SIGNAL(stateChanged(int)), this, SLOT(setAffectRefrLevel(int)));
	connect(affectGlossness, SIGNAL(stateChanged(int)), this, SLOT(setAffectGlossness(int)));
	connect(affectIOR, SIGNAL(stateChanged(int)), this, SLOT(setAffectIOR(int)));
	connect(affectThickness, SIGNAL(stateChanged(int)), this, SLOT(setAffectThickness(int)));

	connect(pureColorRadio, SIGNAL(toggled(bool)), this, SLOT(changeColorPicker(bool)));
	connect(brushModeRadio, SIGNAL(toggled(bool)), this, SLOT(setBrushMode(bool)));
	connect(selectPickerButton, SIGNAL(clicked()), this, SLOT(pickColorPickerFromScene()));
}

PaintSettingWidget::~PaintSettingWidget(void)
{
}


void PaintSettingWidget::setColor( double dummy )
{
	QColor color = colorButton->getColor();
	QVector4D colorV = QVector4D( color.redF(),
		color.greenF(),
		color.blueF(),
		opacitySpinBox->value());
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setColorParam(colorV);
}

void PaintSettingWidget::setColor( QColor color )
{
	QVector4D colorV = QVector4D( color.redF(),
		color.greenF(),
		color.blueF(),
		opacitySpinBox->value());
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setColorParam(colorV);
}

void PaintSettingWidget::setSurface( double dummy )
{
	QVector4D surf = QVector4D( reflLevelSpinBox->value(),
		reflGlossnessSpinBox->value(),
		refrLevelSpinBox->value(),
		refrGlossnessSpinBox->value());
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setSurfParam(surf);

}


void PaintSettingWidget::setThickness( double thick )
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setDepth(thick);
}


void PaintSettingWidget::setAffectColor( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectColor(isAffect);
}

void PaintSettingWidget::setAffectReflLevel( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectReflLevel(isAffect);
}

void PaintSettingWidget::setAffectRefrLevel( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectRefrLevel(isAffect);
}

void PaintSettingWidget::setAffectGlossness( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectGlossness(isAffect);
}

void PaintSettingWidget::setAffectIOR( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectIOR(isAffect);
}

void PaintSettingWidget::setAffectThickness( int isAffect )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAffectThickness(isAffect);
}

void PaintSettingWidget::computeNormalBlendCurve( double dummy )
{
	float v0 = normalV0Spinner->value();
	float v1 = normalV1Spinner->value();
	float w0 = normalW0Spinner->value();
	float w1 = normalW1Spinner->value();
	normalCurveView->setCurve(v0,v1,w0,w1);
	Paint3DFrame::getInstance()->scene->getBrush().setNormalBlendCurveCoef(v0,v1,w0,w1);
}

void PaintSettingWidget::computeDepthBlendCurve( double dummy )
{
	float v0 = depthV0Spinner->value();
	float v1 = depthV1Spinner->value();
	float w0 = depthW0Spinner->value();
	float w1 = depthW1Spinner->value();
	depthCurveView->setCurve(v0,v1,w0,w1);
	Paint3DFrame::getInstance()->scene->getBrush().setDepthBlendCurveCoef(v0,v1,w0,w1);
}

void PaintSettingWidget::setDepthMode( int mode )
{
	Brush& brush = Paint3DFrame::getInstance()->scene->getBrush();
	switch (mode)
	{
	case 0:
		brush.setDepthAffectMode(GeometryExposer::GE_DEPTH_THICKNESS); break;
	case 1:
		brush.setDepthAffectMode(GeometryExposer::GE_DEPTH_GEOMETRY); break;
	}
}


void PaintSettingWidget::changeColorPicker(bool dummy)
{
	if (!isEnabled())
		return;
	bool isPurePicker = pureColorRadio->isChecked();
	QSharedPointer<Scene> scene = Paint3DFrame::getInstance()->scene;
	Brush& brush = scene->getBrush();
	Camera& cam  = scene->getCamera();
	if (isPurePicker)
	{
		attachToCamera(false);
		brush.setDefaultPaintPicker();
	}
	else
	{
		QString curPicker = selectPickerButton->text();
		QSharedPointer<ImagePicker> picker = qSharedPointerCast<ImagePicker>(scene->getObject(curPicker));
		if (picker)
		{
			brush.setCustomPaintPicker(picker);
			attachToCamera(attachToCameraBox->isChecked());
		}
	}
}


void PaintSettingWidget::refreshPickerList()
{
}

void PaintSettingWidget::setColorTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setColorTexture(img);
		colorTexLabel->setPixmap(QPixmap::fromImage(img));
	}
}

void PaintSettingWidget::setReflTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setReflectionTexture(img);
		reflTexLabel->setPixmap(QPixmap::fromImage(convertToGrayScale(img)));
	}
}

void PaintSettingWidget::setGlosTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setGlossnessTexture(img);
		glosTexLabel->setPixmap(QPixmap::fromImage(convertToGrayScale(img)));
	}
}

void PaintSettingWidget::setRefrTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setRefractionTexture(img);
		refrTexLabel->setPixmap(QPixmap::fromImage(convertToGrayScale(img)));
	}
}

void PaintSettingWidget::setIORTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setIORTexture(img);
		iorTexLabel->setPixmap(QPixmap::fromImage(convertToGrayScale(img)));
	}
}

void PaintSettingWidget::setThicknessTex()
{
	QString filename = openImageFile();
	if (!filename.isNull()) 
	{
		//用户选择了文件
		QImage img(filename);
		Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().setThicknessTexture(img);
		thicknessTexLabel->setPixmap(QPixmap::fromImage(convertToGrayScale(img)));
	}
}

void PaintSettingWidget::clearColorTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearColorTexture();
	colorTexLabel->setPixmap(nullPixmap);
}

void PaintSettingWidget::clearReflTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearReflectionTexture();
	reflTexLabel->setPixmap(nullPixmap);
}

void PaintSettingWidget::clearGlosTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearGlossnessTexture();
	glosTexLabel->setPixmap(nullPixmap);
}

void PaintSettingWidget::clearRefrTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearRefractionTexture();
	refrTexLabel->setPixmap(nullPixmap);
}

void PaintSettingWidget::clearIORTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearIORTexture();
	iorTexLabel->setPixmap(nullPixmap);
}

void PaintSettingWidget::clearThicknessTex()
{
	Paint3DFrame::getInstance()->scene->getBrush().getPurePaintPicker().clearThicknessTexture();
	thicknessTexLabel->setPixmap(nullPixmap);
}

QString PaintSettingWidget::openImageFile()
{
	return QFileDialog::getOpenFileName(
		this,
		"Select one image to import",
		QDir::currentPath(),
		"Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff)");
}

QImage PaintSettingWidget::convertToGrayScale( const QImage& img )
{
	QImage resultImg(img.width(), img.height(), QImage::Format_ARGB32);
	for (int y = 0; y < img.height(); ++y)
	{
		for (int x = 0; x < img.width(); ++x)
		{
			int grayScale = qGray(img.pixel(x,y));
			resultImg.setPixel(x,y,qRgba(grayScale, grayScale, grayScale, grayScale));
		}
	}
	return resultImg;
}

void PaintSettingWidget::setPickerOpacity( double opacity )
{
	Brush& brush = Paint3DFrame::getInstance()->scene->getBrush();
	QString curPicker = selectPickerButton->text();
	ImagePicker* picker = (ImagePicker*)Paint3DFrame::getInstance()->scene->getObject(curPicker).data();
	if (picker)
	{
		picker->setOpacity(opacity);
		Paint3DFrame::getInstance()->updateGLView();
	}
}

void PaintSettingWidget::attachToCamera(bool isAttach)
{
	Brush& brush = Paint3DFrame::getInstance()->scene->getBrush();
	QString curPicker = selectPickerButton->text();
	QSharedPointer<RenderableObject> pPicker = Paint3DFrame::getInstance()->scene->getObject(curPicker);
	if (pPicker)
	{
		if (isAttach)
		{
			Camera& cam = Paint3DFrame::getInstance()->scene->getCamera();
			QVector3D trans;
			QQuaternion rot;
			cam.setProjMode(Camera::GLCAMERA_ORTHO);
			cam.addAttachedObjects(pPicker.toWeakRef());
			Paint3DFrame::getInstance()->scene->getScreenExposer().setProjectionType(GeometryExposer::GE_PROJ_ORTHOGRAPHIC);
			Paint3DFrame::getInstance()->updateGLView();
		}
		else
		{
			Camera& cam = Paint3DFrame::getInstance()->scene->getCamera();
			QVector3D trans;
			QQuaternion rot;
			cam.setProjMode(Camera::GLCAMERA_PERSP);
			cam.clearAttachedObjects();
			Paint3DFrame::getInstance()->scene->getScreenExposer().setProjectionType(GeometryExposer::GE_PROJ_PERSPECTIVE);
			Paint3DFrame::getInstance()->updateGLView();
		}
	}
}

void PaintSettingWidget::detachFromCamera()
{
}

void PaintSettingWidget::updateWidgets()
{
}

void PaintSettingWidget::pickColorPickerFromScene()
{
	Paint3DFrame::getInstance()->viewWidget->enterPickerMode(getPickedObject);
}

void getPickedObject( QSharedPointer<RenderableObject> obj )
{
	PaintSettingWidget& thisEditor = *Paint3DFrame::getInstance()->paintEditor;
	if (obj && obj->getType() == RenderableObject::OBJ_PICKER_OBJECT)
	{
		thisEditor.selectPickerButton->setChecked(false);
		thisEditor.attachToCameraBox->setChecked(false);
		thisEditor.selectPickerButton->setText(obj->getName());
		Paint3DFrame::getInstance()->scene->getBrush().setCustomPaintPicker(qSharedPointerCast<ImagePicker>(obj));
	}
	Paint3DFrame::getInstance()->viewWidget->setCursor(Qt::ArrowCursor);
}

void PaintSettingWidget::setAttachingParam( double dummy )
{
	float xOffset = xOffsetSpinBox->value();
	float yOffset = yOffsetSpinBox->value();
	float zOffset = zOffsetSpinBox->value();
	float rot     = rotateSpinBox->value();
	float xScale  = xScaleSpinBox->value();
	float yScale  = yScaleSpinBox->value();
	Camera& cam = Paint3DFrame::getInstance()->scene->getCamera();
	cam.setAttachObjectOffset(xOffset, yOffset, zOffset);
	cam.setAttachObjectRotation(rot);
	cam.setAttachObjectScale(xScale, yScale);
	Paint3DFrame::getInstance()->viewWidget->updateGL();
}

void PaintSettingWidget::setBrushMode( bool isBrushMode )
{
	if (isBrushMode)
	{
		Paint3DFrame::getInstance()->scene->getBrush().setMode(Brush::MODE_BRUSH);
	}
	else
	{
		Paint3DFrame::getInstance()->scene->getBrush().setMode(Brush::MODE_ERASER);
	}
}
