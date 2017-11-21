#include "stdafx.h"
#include "paint3dframe.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QDir::setCurrent(QCoreApplication::applicationDirPath ());
	//qDebug()<<QCoreApplication::applicationDirPath ();
	QTranslator *translator = new QTranslator;
	translator->load("paint3d_zh.qm");
	a.installTranslator(translator);
	Paint3DFrame* paint3DApp = Paint3DFrame::getInstance();
	paint3DApp->show();
	return a.exec();
}
