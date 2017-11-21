#pragma once
#include <QtWidgets/QPushButton>

class ColorSelectButton:public QPushButton
{
	Q_OBJECT
public:
	ColorSelectButton( QWidget * parent = 0 );
	~ColorSelectButton(void);

	QColor getColor(){return color;}
signals:
	void colorChanged(QColor color);

private slots:
	void selectColor();
protected:
	void paintEvent(QPaintEvent *event);
private:
	QColor color;
};
