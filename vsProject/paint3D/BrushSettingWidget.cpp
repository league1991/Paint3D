#include "StdAfx.h"
#include "BrushSettingWidget.h"
#include "paint3DFrame.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
class Paint3DFrame;

BrushSettingWidget::BrushSettingWidget( QWidget *parent /*= 0*/, Qt::WindowFlags flags /*= 0*/ )
{
	setupUi(this);

	connect(sizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSize(double)));
	connect(resolutionRatioSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setResolutionRatio(double)));
	connect(angleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));
	connect(ratioSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRatio(double)));
	connect(sizeJitterSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSizeJitter(double)));
	connect(angleJitterSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngleJitter(double)));
	connect(ratioJitterSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRatioJitter(double)));
	connect(offsetRangeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setOffsetJitter(double)));
	connect(intervalSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setInterval(double)));
	connect(nearPlaneSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDepthRange(double)));
	connect(farPlaneSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDepthRange(double)));


	connect(hardnessSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHardness(double)));

	connect(loadFromImageButton, SIGNAL(clicked()), this, SLOT(loadStrokeFromImage()));
	connect(loadFromAbrButton, SIGNAL(clicked()), this, SLOT(loadStrokeFromAbr()));
	connect(strokeList, SIGNAL(currentRowChanged(int)), this, SLOT(setStroke(int)));

}

BrushSettingWidget::~BrushSettingWidget( void )
{

}

void BrushSettingWidget::setHardness( double hard )
{
	QSharedPointer<Stroke> pStroke = Paint3DFrame::getInstance()->scene->getBrush().getStroke();
	if (pStroke->getType() == Stroke::STROKE_ANALYTIC)
	{
		qSharedPointerCast<StandardStroke>(pStroke)->setHardness(hard);
	}
}


void BrushSettingWidget::setSize( double size )
{
	Paint3DFrame::getInstance()->scene->getBrush().setSize(size);
}

void BrushSettingWidget::setAngle( double angle )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAngle(angle / 180 * M_PI);
}

void BrushSettingWidget::setRatio( double ratio )
{
	Paint3DFrame::getInstance()->scene->getBrush().setRatio(ratio);
}

void BrushSettingWidget::setSizeJitter( double sizeJitter )
{
	Paint3DFrame::getInstance()->scene->getBrush().setSizeJitter(sizeJitter);
}

void BrushSettingWidget::setAngleJitter( double angleJitter )
{
	Paint3DFrame::getInstance()->scene->getBrush().setAngleJitter(angleJitter);
}

void BrushSettingWidget::setRatioJitter( double ratioJitter )
{
	Paint3DFrame::getInstance()->scene->getBrush().setRatioJitter(ratioJitter);
}

void BrushSettingWidget::setOffsetJitter( double offsetJitter )
{
	Paint3DFrame::getInstance()->scene->getBrush().setOffsetJitter(offsetJitter);
}

void BrushSettingWidget::setInterval( double interval )
{
	Paint3DFrame::getInstance()->scene->getBrush().setIntervalRatio(interval);
}


void BrushSettingWidget::refreshListViewItem()
{
	strokeList->clear();
	StrokeLib& strokeLib = Paint3DFrame::getInstance()->scene->getStrokeLib();
	for (int ithItem = 0; ithItem < strokeLib.getNumStrokes(); ++ithItem)
	{
		QListWidgetItem* item = new QListWidgetItem;
		QSharedPointer<Stroke> stroke = strokeLib.getStroke(ithItem);
		item->setText(stroke->getName());
		item->setIcon(stroke->getIcon());
		item->setTextAlignment(Qt::AlignRight);
		item->setSizeHint(QSize(40,40));
		strokeList->addItem(item);
	}
	strokeList->setIconSize(QSize(40,40));
}

void BrushSettingWidget::loadStrokeFromImage()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		QString::fromUtf8("Open Brush Stroke"),
		QDir::currentPath(),
		"Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff)");

	if (!filename.isNull()) 
	{ //用户选择了文件
		if (!Paint3DFrame::getInstance()->scene->getStrokeLib().loadFromImage(filename))
		{
			QMessageBox::information(this, tr("import"), tr("Not a valid obj file."), QMessageBox::Ok);
		}
	}
	refreshListViewItem();
}

void BrushSettingWidget::loadStrokeFromAbr()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		QString::fromUtf8("Open Brush Stroke"),
		QDir::currentPath(),
		"Abr files (*.abr)");
	if (!filename.isNull()) 
	{ //用户选择了文件
		if (!Paint3DFrame::getInstance()->scene->getStrokeLib().loadFromAbr(filename))
		{
			QMessageBox::information(this, tr("import"), tr("Not a valid obj file."), QMessageBox::Ok);
		}
	}
	refreshListViewItem();
}

void BrushSettingWidget::setStroke( int ithStroke )
{
	ithStroke = ithStroke == -1 ? 0 : ithStroke;
	QSharedPointer<Stroke> ps = Paint3DFrame::getInstance()->scene->getStrokeLib().getStroke(ithStroke);
	Paint3DFrame::getInstance()->scene->getBrush().setStroke(ps);
}

void BrushSettingWidget::setResolutionRatio( double ratio )
{
	Paint3DFrame::getInstance()->scene->getBrush().setResolutionRatio(ratio);
}

void BrushSettingWidget::setDepthRange( double dummy )
{
	Paint3DFrame::getInstance()->scene->getBrush().setDepthRange(nearPlaneSpinBox->value(), farPlaneSpinBox->value());
}
