/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <historywidget.h>
#include <paintsettingwidget.h>
#include "brushsettingwidget.h"
#include "glviewwidget.h"
#include "materiallayerwidget.h"
#include "objecttransformwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImport;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionMove;
    QAction *actionRotate;
    QAction *actionScale;
    QAction *actionSelect;
    QAction *actionFocus;
    QAction *actionPaint;
    QAction *actionPlaneLocator;
    QAction *actionSelectFace;
    QAction *actionAbout;
    GLViewWidget *viewWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuCreate;
    QMenu *menuHelp;
    QMenu *menuWindow;
    QStatusBar *statusBar;
    QToolBar *fileToolBar;
    QDockWidget *objectDocker;
    ObjectTransformWidget *transformEditor;
    QToolBar *manipulateToolBar;
    QDockWidget *historyDocker;
    HistoryWidget *historyEditor;
    QDockWidget *layerDocker;
    MaterialLayerWidget *layerEditor;
    QDockWidget *paintEditorDocker;
    PaintSettingWidget *paintEditor;
    QToolBar *createToolBar;
    QDockWidget *brushEditorDocker;
    BrushSettingWidget *brushEditor;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(616, 571);
        MainWindow->setMouseTracking(true);
        MainWindow->setFocusPolicy(Qt::ClickFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/paint3D/Resources/software.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDocumentMode(false);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/paint3D/Resources/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImport->setIcon(icon1);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/paint3D/Resources/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon2);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/paint3D/Resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon3);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/paint3D/Resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon4);
        actionMove = new QAction(MainWindow);
        actionMove->setObjectName(QStringLiteral("actionMove"));
        actionMove->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/paint3D/Resources/move.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMove->setIcon(icon5);
        actionRotate = new QAction(MainWindow);
        actionRotate->setObjectName(QStringLiteral("actionRotate"));
        actionRotate->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/paint3D/Resources/rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate->setIcon(icon6);
        actionScale = new QAction(MainWindow);
        actionScale->setObjectName(QStringLiteral("actionScale"));
        actionScale->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/paint3D/Resources/scale.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScale->setIcon(icon7);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QStringLiteral("actionSelect"));
        actionSelect->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/paint3D/Resources/select.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect->setIcon(icon8);
        actionFocus = new QAction(MainWindow);
        actionFocus->setObjectName(QStringLiteral("actionFocus"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/paint3D/Resources/focus.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFocus->setIcon(icon9);
        actionPaint = new QAction(MainWindow);
        actionPaint->setObjectName(QStringLiteral("actionPaint"));
        actionPaint->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/paint3D/Resources/brush.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaint->setIcon(icon10);
        actionPlaneLocator = new QAction(MainWindow);
        actionPlaneLocator->setObjectName(QStringLiteral("actionPlaneLocator"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/paint3D/Resources/planeLocator .png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlaneLocator->setIcon(icon11);
        actionSelectFace = new QAction(MainWindow);
        actionSelectFace->setObjectName(QStringLiteral("actionSelectFace"));
        actionSelectFace->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/paint3D/Resources/selectFace.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectFace->setIcon(icon12);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        viewWidget = new GLViewWidget(MainWindow);
        viewWidget->setObjectName(QStringLiteral("viewWidget"));
        MainWindow->setCentralWidget(viewWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 616, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuCreate = new QMenu(menuBar);
        menuCreate->setObjectName(QStringLiteral("menuCreate"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        fileToolBar = new QToolBar(MainWindow);
        fileToolBar->setObjectName(QStringLiteral("fileToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, fileToolBar);
        objectDocker = new QDockWidget(MainWindow);
        objectDocker->setObjectName(QStringLiteral("objectDocker"));
        objectDocker->setEnabled(true);
        objectDocker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        transformEditor = new ObjectTransformWidget();
        transformEditor->setObjectName(QStringLiteral("transformEditor"));
        transformEditor->setEnabled(false);
        objectDocker->setWidget(transformEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), objectDocker);
        manipulateToolBar = new QToolBar(MainWindow);
        manipulateToolBar->setObjectName(QStringLiteral("manipulateToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, manipulateToolBar);
        historyDocker = new QDockWidget(MainWindow);
        historyDocker->setObjectName(QStringLiteral("historyDocker"));
        historyDocker->setFloating(false);
        historyDocker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        historyEditor = new HistoryWidget();
        historyEditor->setObjectName(QStringLiteral("historyEditor"));
        historyDocker->setWidget(historyEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), historyDocker);
        layerDocker = new QDockWidget(MainWindow);
        layerDocker->setObjectName(QStringLiteral("layerDocker"));
        layerDocker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        layerEditor = new MaterialLayerWidget();
        layerEditor->setObjectName(QStringLiteral("layerEditor"));
        layerDocker->setWidget(layerEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), layerDocker);
        paintEditorDocker = new QDockWidget(MainWindow);
        paintEditorDocker->setObjectName(QStringLiteral("paintEditorDocker"));
        paintEditorDocker->setEnabled(true);
        paintEditorDocker->setAllowedAreas(Qt::AllDockWidgetAreas);
        paintEditor = new PaintSettingWidget();
        paintEditor->setObjectName(QStringLiteral("paintEditor"));
        paintEditor->setEnabled(false);
        paintEditorDocker->setWidget(paintEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), paintEditorDocker);
        createToolBar = new QToolBar(MainWindow);
        createToolBar->setObjectName(QStringLiteral("createToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, createToolBar);
        brushEditorDocker = new QDockWidget(MainWindow);
        brushEditorDocker->setObjectName(QStringLiteral("brushEditorDocker"));
        brushEditorDocker->setEnabled(true);
        brushEditorDocker->setFeatures(QDockWidget::AllDockWidgetFeatures);
        brushEditorDocker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        brushEditor = new BrushSettingWidget();
        brushEditor->setObjectName(QStringLiteral("brushEditor"));
        brushEditorDocker->setWidget(brushEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), brushEditorDocker);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuCreate->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionImport);
        menuEdit->addAction(actionSelect);
        menuEdit->addAction(actionSelectFace);
        menuEdit->addSeparator();
        menuEdit->addAction(actionMove);
        menuEdit->addAction(actionRotate);
        menuEdit->addAction(actionScale);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFocus);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPaint);
        menuEdit->addSeparator();
        menuCreate->addAction(actionImport);
        menuCreate->addAction(actionPlaneLocator);
        menuHelp->addAction(actionAbout);
        fileToolBar->addAction(actionNew);
        fileToolBar->addAction(actionOpen);
        fileToolBar->addAction(actionSave);
        fileToolBar->addSeparator();
        manipulateToolBar->addAction(actionSelect);
        manipulateToolBar->addAction(actionSelectFace);
        manipulateToolBar->addSeparator();
        manipulateToolBar->addAction(actionMove);
        manipulateToolBar->addAction(actionRotate);
        manipulateToolBar->addAction(actionScale);
        manipulateToolBar->addSeparator();
        manipulateToolBar->addAction(actionFocus);
        manipulateToolBar->addSeparator();
        manipulateToolBar->addAction(actionPaint);
        createToolBar->addAction(actionImport);
        createToolBar->addAction(actionPlaneLocator);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Paint3D", 0));
        actionImport->setText(QApplication::translate("MainWindow", "Import Model", 0));
#ifndef QT_NO_TOOLTIP
        actionImport->setToolTip(QApplication::translate("MainWindow", "Import a 3d Model", 0));
#endif // QT_NO_TOOLTIP
        actionImport->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("MainWindow", "Create a New File", 0));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "Open a File", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "Save File", 0));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionMove->setText(QApplication::translate("MainWindow", "Move", 0));
#ifndef QT_NO_TOOLTIP
        actionMove->setToolTip(QApplication::translate("MainWindow", "Move Object", 0));
#endif // QT_NO_TOOLTIP
        actionMove->setShortcut(QApplication::translate("MainWindow", "W", 0));
        actionRotate->setText(QApplication::translate("MainWindow", "Rotate", 0));
#ifndef QT_NO_TOOLTIP
        actionRotate->setToolTip(QApplication::translate("MainWindow", "Rotate Object", 0));
#endif // QT_NO_TOOLTIP
        actionRotate->setShortcut(QApplication::translate("MainWindow", "E", 0));
        actionScale->setText(QApplication::translate("MainWindow", "Scale", 0));
#ifndef QT_NO_TOOLTIP
        actionScale->setToolTip(QApplication::translate("MainWindow", "Scale Object", 0));
#endif // QT_NO_TOOLTIP
        actionScale->setShortcut(QApplication::translate("MainWindow", "R", 0));
        actionSelect->setText(QApplication::translate("MainWindow", "Select", 0));
#ifndef QT_NO_TOOLTIP
        actionSelect->setToolTip(QApplication::translate("MainWindow", "Select Object", 0));
#endif // QT_NO_TOOLTIP
        actionSelect->setShortcut(QApplication::translate("MainWindow", "Q", 0));
        actionFocus->setText(QApplication::translate("MainWindow", "Focus Model", 0));
#ifndef QT_NO_TOOLTIP
        actionFocus->setToolTip(QApplication::translate("MainWindow", "Focus Selected Model", 0));
#endif // QT_NO_TOOLTIP
        actionFocus->setShortcut(QApplication::translate("MainWindow", "F", 0));
        actionPaint->setText(QApplication::translate("MainWindow", "Paint", 0));
#ifndef QT_NO_TOOLTIP
        actionPaint->setToolTip(QApplication::translate("MainWindow", "Paint Model", 0));
#endif // QT_NO_TOOLTIP
        actionPaint->setShortcut(QApplication::translate("MainWindow", "B", 0));
        actionPlaneLocator->setText(QApplication::translate("MainWindow", "Create Plane Locator", 0));
#ifndef QT_NO_TOOLTIP
        actionPlaneLocator->setToolTip(QApplication::translate("MainWindow", "create a plane locator", 0));
#endif // QT_NO_TOOLTIP
        actionSelectFace->setText(QApplication::translate("MainWindow", "Select Face", 0));
#ifndef QT_NO_TOOLTIP
        actionSelectFace->setToolTip(QApplication::translate("MainWindow", "Select Face", 0));
#endif // QT_NO_TOOLTIP
        actionSelectFace->setShortcut(QApplication::translate("MainWindow", "A", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuCreate->setTitle(QApplication::translate("MainWindow", "Create", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0));
        fileToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        objectDocker->setWindowTitle(QApplication::translate("MainWindow", "Object", 0));
        manipulateToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
        historyDocker->setWindowTitle(QApplication::translate("MainWindow", "History", 0));
        layerDocker->setWindowTitle(QApplication::translate("MainWindow", "Material Layer", 0));
        paintEditorDocker->setWindowTitle(QApplication::translate("MainWindow", "Paint", 0));
        createToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        brushEditorDocker->setWindowTitle(QApplication::translate("MainWindow", "Brush", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
