#include "StdAfx.h"
#include "ColorSelectButton.h"
#include <QtWidgets/QColorDialog>


ColorSelectButton::ColorSelectButton( QWidget * parent /*= 0 */ ):QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(selectColor()));
}

ColorSelectButton::~ColorSelectButton(void)
{
}

void ColorSelectButton::selectColor()
{
		QColorDialog dialog(color, 0);
		dialog.setOption(QColorDialog::ShowAlphaChannel, false);
		// The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
#ifdef Q_WS_MAC
		dialog.setOption(QColorDialog::DontUseNativeDialog, true);
#endif
		dialog.move(280, 120);
		if (dialog.exec() == QDialog::Rejected)
			return;
		color = dialog.selectedColor();
		emit colorChanged(color);
}

void ColorSelectButton::paintEvent( QPaintEvent *event )
{
	QPainter painter(this);
	QSize size = this->size();
	painter.fillRect(0,0,size.width(),size.height(),color);
}
