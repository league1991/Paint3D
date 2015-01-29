#include "stdafx.h"
#include "paint3dframe.h"
#include <QtGui/QApplication>
Paint3DFrame* paint3DApp;

QSharedPointer<Scene> getScene()
{
	return paint3DApp->scene;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QDir::setCurrent(QCoreApplication::applicationDirPath ());
	//qDebug()<<QCoreApplication::applicationDirPath ();
	QTranslator *translator = new QTranslator;
	translator->load("paint3d_zh.qm");
	a.installTranslator(translator);
	paint3DApp = new Paint3DFrame;
	paint3DApp->show();
	return a.exec();
}
