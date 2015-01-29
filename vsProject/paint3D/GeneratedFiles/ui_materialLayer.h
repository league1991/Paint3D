/********************************************************************************
** Form generated from reading UI file 'materialLayer.ui'
**
** Created: Mon Sep 22 21:18:39 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALLAYER_H
#define UI_MATERIALLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialLayer
{
public:
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QListWidget *layerList;
    QHBoxLayout *horizontalLayout;
    QPushButton *addLayerButton;
    QPushButton *deleteLayerButton;
    QPushButton *moveUpButton;
    QPushButton *moveDownButton;
    QGroupBox *channelGroupBox;
    QGridLayout *gridLayout;
    QRadioButton *colorBox;
    QRadioButton *reflectionBox;
    QRadioButton *glossnessBox;
    QRadioButton *refractionBox;
    QRadioButton *iorBox;
    QRadioButton *thicknessBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *importChannelButton;
    QPushButton *exportImageButton;
    QPushButton *exportFinalImageButton;

    void setupUi(QScrollArea *MaterialLayer)
    {
        if (MaterialLayer->objectName().isEmpty())
            MaterialLayer->setObjectName(QString::fromUtf8("MaterialLayer"));
        MaterialLayer->resize(409, 523);
        MaterialLayer->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 407, 521));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        layerList = new QListWidget(scrollAreaWidgetContents);
        layerList->setObjectName(QString::fromUtf8("layerList"));
        QFont font;
        font.setPointSize(8);
        layerList->setFont(font);
        layerList->setAlternatingRowColors(false);
        layerList->setSelectionBehavior(QAbstractItemView::SelectRows);
        layerList->setViewMode(QListView::ListMode);
        layerList->setSelectionRectVisible(true);

        verticalLayout->addWidget(layerList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addLayerButton = new QPushButton(scrollAreaWidgetContents);
        addLayerButton->setObjectName(QString::fromUtf8("addLayerButton"));

        horizontalLayout->addWidget(addLayerButton);

        deleteLayerButton = new QPushButton(scrollAreaWidgetContents);
        deleteLayerButton->setObjectName(QString::fromUtf8("deleteLayerButton"));

        horizontalLayout->addWidget(deleteLayerButton);

        moveUpButton = new QPushButton(scrollAreaWidgetContents);
        moveUpButton->setObjectName(QString::fromUtf8("moveUpButton"));

        horizontalLayout->addWidget(moveUpButton);

        moveDownButton = new QPushButton(scrollAreaWidgetContents);
        moveDownButton->setObjectName(QString::fromUtf8("moveDownButton"));

        horizontalLayout->addWidget(moveDownButton);


        verticalLayout->addLayout(horizontalLayout);

        channelGroupBox = new QGroupBox(scrollAreaWidgetContents);
        channelGroupBox->setObjectName(QString::fromUtf8("channelGroupBox"));
        gridLayout = new QGridLayout(channelGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        colorBox = new QRadioButton(channelGroupBox);
        colorBox->setObjectName(QString::fromUtf8("colorBox"));
        colorBox->setChecked(true);

        gridLayout->addWidget(colorBox, 0, 0, 1, 1);

        reflectionBox = new QRadioButton(channelGroupBox);
        reflectionBox->setObjectName(QString::fromUtf8("reflectionBox"));

        gridLayout->addWidget(reflectionBox, 0, 1, 1, 1);

        glossnessBox = new QRadioButton(channelGroupBox);
        glossnessBox->setObjectName(QString::fromUtf8("glossnessBox"));

        gridLayout->addWidget(glossnessBox, 1, 0, 1, 1);

        refractionBox = new QRadioButton(channelGroupBox);
        refractionBox->setObjectName(QString::fromUtf8("refractionBox"));

        gridLayout->addWidget(refractionBox, 1, 1, 1, 1);

        iorBox = new QRadioButton(channelGroupBox);
        iorBox->setObjectName(QString::fromUtf8("iorBox"));

        gridLayout->addWidget(iorBox, 2, 0, 1, 1);

        thicknessBox = new QRadioButton(channelGroupBox);
        thicknessBox->setObjectName(QString::fromUtf8("thicknessBox"));

        gridLayout->addWidget(thicknessBox, 2, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        importChannelButton = new QPushButton(channelGroupBox);
        importChannelButton->setObjectName(QString::fromUtf8("importChannelButton"));

        horizontalLayout_2->addWidget(importChannelButton);

        exportImageButton = new QPushButton(channelGroupBox);
        exportImageButton->setObjectName(QString::fromUtf8("exportImageButton"));

        horizontalLayout_2->addWidget(exportImageButton);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 2);

        exportFinalImageButton = new QPushButton(channelGroupBox);
        exportFinalImageButton->setObjectName(QString::fromUtf8("exportFinalImageButton"));

        gridLayout->addWidget(exportFinalImageButton, 4, 0, 1, 2);


        verticalLayout->addWidget(channelGroupBox);

        MaterialLayer->setWidget(scrollAreaWidgetContents);

        retranslateUi(MaterialLayer);

        QMetaObject::connectSlotsByName(MaterialLayer);
    } // setupUi

    void retranslateUi(QScrollArea *MaterialLayer)
    {
        MaterialLayer->setWindowTitle(QApplication::translate("MaterialLayer", "Layer", 0, QApplication::UnicodeUTF8));
        addLayerButton->setText(QApplication::translate("MaterialLayer", "Add Layer", 0, QApplication::UnicodeUTF8));
        deleteLayerButton->setText(QApplication::translate("MaterialLayer", "Delete Layer", 0, QApplication::UnicodeUTF8));
        moveUpButton->setText(QApplication::translate("MaterialLayer", "Move Up", 0, QApplication::UnicodeUTF8));
        moveDownButton->setText(QApplication::translate("MaterialLayer", "Move Down", 0, QApplication::UnicodeUTF8));
        channelGroupBox->setTitle(QApplication::translate("MaterialLayer", "Channel", 0, QApplication::UnicodeUTF8));
        colorBox->setText(QApplication::translate("MaterialLayer", "Color", 0, QApplication::UnicodeUTF8));
        reflectionBox->setText(QApplication::translate("MaterialLayer", "Reflection", 0, QApplication::UnicodeUTF8));
        glossnessBox->setText(QApplication::translate("MaterialLayer", "Glossness", 0, QApplication::UnicodeUTF8));
        refractionBox->setText(QApplication::translate("MaterialLayer", "Refraction", 0, QApplication::UnicodeUTF8));
        iorBox->setText(QApplication::translate("MaterialLayer", "IOR", 0, QApplication::UnicodeUTF8));
        thicknessBox->setText(QApplication::translate("MaterialLayer", "Thickness", 0, QApplication::UnicodeUTF8));
        importChannelButton->setText(QApplication::translate("MaterialLayer", "Import Image to Layer", 0, QApplication::UnicodeUTF8));
        exportImageButton->setText(QApplication::translate("MaterialLayer", "Export Layer to Image", 0, QApplication::UnicodeUTF8));
        exportFinalImageButton->setText(QApplication::translate("MaterialLayer", "Export Canvas to Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MaterialLayer: public Ui_MaterialLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALLAYER_H
