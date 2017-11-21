#ifndef PAINT3D_H
#define PAINT3D_H

#include <QtGui/QMainWindow>
#include "ui_mainWindow.h"

class Paint3DFrame : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	~Paint3DFrame();

	static Paint3DFrame* getInstance();
	static QSharedPointer<Scene> scene;
	Scene* getScene(){return scene.data();}
	void initialize();
	void updateGLView();
private slots:
	void importModel();
	void selectTool();
	void selectFaceTool();
	void moveTool();
	void rotateTool();
	void scaleTool();
	void focusTool();
	void paintTool();
	void openFile();
	void saveFile();
	void newFile();

	void refreshView();
	void createPlaneLocator();

	void showAboutWindow();
private:
	Paint3DFrame(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	static Paint3DFrame* instance;
	QActionGroup *actionGroup;
	QAction* undoAction;
	QAction* redoAction;
};

#endif // PAINT3D_H
