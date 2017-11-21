#include "stdafx.h"
#include "paint3dframe.h"
Paint3DFrame* Paint3DFrame::instance = NULL;
QSharedPointer<Scene> Paint3DFrame::scene = QSharedPointer<Scene>(new Scene);

Paint3DFrame::Paint3DFrame(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	setupUi(this);

	QIcon undoIcon, redoIcon;
	undoIcon.addFile(QString::fromUtf8(":/paint3D/Resources/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
	redoIcon.addFile(QString::fromUtf8(":/paint3D/Resources/redo.png"), QSize(), QIcon::Normal, QIcon::Off);

	undoAction = scene->getUndoStack().createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);
	undoAction->setIcon(undoIcon);

	redoAction = scene->getUndoStack().createRedoAction(this, tr("&Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);
	redoAction->setIcon(redoIcon);

	menuEdit->addAction(undoAction);
	menuEdit->addAction(redoAction);
	menuEdit->addSeparator();

	menuWindow->addAction(brushEditorDocker->toggleViewAction());
	menuWindow->addAction(paintEditorDocker->toggleViewAction());
	menuWindow->addAction(objectDocker->toggleViewAction());
	menuWindow->addAction(historyDocker->toggleViewAction());
	menuWindow->addAction(layerDocker->toggleViewAction());

	fileToolBar->addAction(undoAction);
	fileToolBar->addAction(redoAction);

	actionGroup = new QActionGroup(this);
	actionGroup->addAction(actionSelect);
	actionGroup->addAction(actionMove);
	actionGroup->addAction(actionRotate);
	actionGroup->addAction(actionScale);
	actionGroup->addAction(actionPaint);
	actionGroup->addAction(actionSelectFace);
	actionGroup->setExclusive(true);

	connect(actionImport, SIGNAL(triggered()), this, SLOT(importModel()));
	connect(actionSelect, SIGNAL(triggered()), this, SLOT(selectTool()));
	connect(actionSelectFace, SIGNAL(triggered()), this, SLOT(selectFaceTool()));
	connect(actionMove, SIGNAL(triggered()), this, SLOT(moveTool()));
	connect(actionRotate, SIGNAL(triggered()), this, SLOT(rotateTool()));
	connect(actionScale, SIGNAL(triggered()), this, SLOT(scaleTool()));
	connect(actionFocus, SIGNAL(triggered()), this, SLOT(focusTool()));
	connect(actionPaint, SIGNAL(triggered()), this, SLOT(paintTool()));
	connect(actionPlaneLocator, SIGNAL(triggered()), this, SLOT(createPlaneLocator()));
	connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	

	connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));


	actionSelect->setChecked(true);
}

Paint3DFrame::~Paint3DFrame()
{

}
void Paint3DFrame::createPlaneLocator()
{
	scene->addPlanePicker();
	paintEditor->refreshPickerList();
}
void Paint3DFrame::importModel()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Document",
		QDir::currentPath(),
		"Obj files (*.obj)");
	if (!filename.isNull()) 
	{ //用户选择了文件
		if (!scene->importObj(filename))
		{
			QMessageBox::information(this, tr("import"), tr("Not a valid obj file."), QMessageBox::Ok);
		}
	}
}

void Paint3DFrame::selectTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_SELECT);
	centralWidget()->update();
}

void Paint3DFrame::moveTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_TRANSLATE);
	centralWidget()->update();
}

void Paint3DFrame::rotateTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_ROTATE);
	centralWidget()->update();
}

void Paint3DFrame::scaleTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_SCALE);
	centralWidget()->update();
}

void Paint3DFrame::focusTool()
{
	viewWidget->focusCurSelected();
	centralWidget()->update();
}

void Paint3DFrame::paintTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_PAINT);
	centralWidget()->update();
}

void Paint3DFrame::refreshView()
{
	viewWidget->updateGL();
}

void Paint3DFrame::updateGLView()
{
	viewWidget->updateGL();
}

void Paint3DFrame::selectFaceTool()
{
	viewWidget->setTool(GLViewWidget::TOOL_FACE_SELECT);
	centralWidget()->update();
}

void Paint3DFrame::openFile()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		tr("Open Document"),
		QDir::currentPath(),
		"Paint3D Document (*.p3d)");
	scene->open(filename);
	centralWidget()->update();
}

void Paint3DFrame::saveFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
		QDir::currentPath(),
		"Paint3D Document (*.p3d)");
	scene->save(filename);
}

void Paint3DFrame::newFile()
{
	scene->clear();
	centralWidget()->update();
}

void Paint3DFrame::showAboutWindow()
{
	QMessageBox::about ( this, tr("About..."), tr("Paint3D v1.0 \nCopyright(C) 2012\nOuyang Yaobin") );
}

