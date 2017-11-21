/********************************************************************************
** Form generated from reading UI file 'materialLayer.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALLAYER_H
#define UI_MATERIALLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
            MaterialLayer->setObjectName(QStringLiteral("MaterialLayer"));
        MaterialLayer->resize(409, 523);
        MaterialLayer->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 407, 521));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        layerList = new QListWidget(scrollAreaWidgetContents);
        layerList->setObjectName(QStringLiteral("layerList"));
        QFont font;
        font.setPointSize(8);
        layerList->setFont(font);
        layerList->setAlternatingRowColors(false);
        layerList->setSelectionBehavior(QAbstractItemView::SelectRows);
        layerList->setViewMode(QListView::ListMode);
        layerList->setSelectionRectVisible(true);

        verticalLayout->addWidget(layerList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        addLayerButton = new QPushButton(scrollAreaWidgetContents);
        addLayerButton->setObjectName(QStringLiteral("addLayerButton"));

        horizontalLayout->addWidget(addLayerButton);

        deleteLayerButton = new QPushButton(scrollAreaWidgetContents);
        deleteLayerButton->setObjectName(QStringLiteral("deleteLayerButton"));

        horizontalLayout->addWidget(deleteLayerButton);

        moveUpButton = new QPushButton(scrollAreaWidgetContents);
        moveUpButton->setObjectName(QStringLiteral("moveUpButton"));

        horizontalLayout->addWidget(moveUpButton);

        moveDownButton = new QPushButton(scrollAreaWidgetContents);
        moveDownButton->setObjectName(QStringLiteral("moveDownButton"));

        horizontalLayout->addWidget(moveDownButton);


        verticalLayout->addLayout(horizontalLayout);

        channelGroupBox = new QGroupBox(scrollAreaWidgetContents);
        channelGroupBox->setObjectName(QStringLiteral("channelGroupBox"));
        gridLayout = new QGridLayout(channelGroupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        colorBox = new QRadioButton(channelGroupBox);
        colorBox->setObjectName(QStringLiteral("colorBox"));
        colorBox->setChecked(true);

        gridLayout->addWidget(colorBox, 0, 0, 1, 1);

        reflectionBox = new QRadioButton(channelGroupBox);
        reflectionBox->setObjectName(QStringLiteral("reflectionBox"));

        gridLayout->addWidget(reflectionBox, 0, 1, 1, 1);

        glossnessBox = new QRadioButton(channelGroupBox);
        glossnessBox->setObjectName(QStringLiteral("glossnessBox"));

        gridLayout->addWidget(glossnessBox, 1, 0, 1, 1);

        refractionBox = new QRadioButton(channelGroupBox);
        refractionBox->setObjectName(QStringLiteral("refractionBox"));

        gridLayout->addWidget(refractionBox, 1, 1, 1, 1);

        iorBox = new QRadioButton(channelGroupBox);
        iorBox->setObjectName(QStringLiteral("iorBox"));

        gridLayout->addWidget(iorBox, 2, 0, 1, 1);

        thicknessBox = new QRadioButton(channelGroupBox);
        thicknessBox->setObjectName(QStringLiteral("thicknessBox"));

        gridLayout->addWidget(thicknessBox, 2, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        importChannelButton = new QPushButton(channelGroupBox);
        importChannelButton->setObjectName(QStringLiteral("importChannelButton"));

        horizontalLayout_2->addWidget(importChannelButton);

        exportImageButton = new QPushButton(channelGroupBox);
        exportImageButton->setObjectName(QStringLiteral("exportImageButton"));

        horizontalLayout_2->addWidget(exportImageButton);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 2);

        exportFinalImageButton = new QPushButton(channelGroupBox);
        exportFinalImageButton->setObjectName(QStringLiteral("exportFinalImageButton"));

        gridLayout->addWidget(exportFinalImageButton, 4, 0, 1, 2);


        verticalLayout->addWidget(channelGroupBox);

        MaterialLayer->setWidget(scrollAreaWidgetContents);

        retranslateUi(MaterialLayer);

        QMetaObject::connectSlotsByName(MaterialLayer);
    } // setupUi

    void retranslateUi(QScrollArea *MaterialLayer)
    {
        MaterialLayer->setWindowTitle(QApplication::translate("MaterialLayer", "Layer", 0));
        addLayerButton->setText(QApplication::translate("MaterialLayer", "Add Layer", 0));
        deleteLayerButton->setText(QApplication::translate("MaterialLayer", "Delete Layer", 0));
        moveUpButton->setText(QApplication::translate("MaterialLayer", "Move Up", 0));
        moveDownButton->setText(QApplication::translate("MaterialLayer", "Move Down", 0));
        channelGroupBox->setTitle(QApplication::translate("MaterialLayer", "Channel", 0));
        colorBox->setText(QApplication::translate("MaterialLayer", "Color", 0));
        reflectionBox->setText(QApplication::translate("MaterialLayer", "Reflection", 0));
        glossnessBox->setText(QApplication::translate("MaterialLayer", "Glossness", 0));
        refractionBox->setText(QApplication::translate("MaterialLayer", "Refraction", 0));
        iorBox->setText(QApplication::translate("MaterialLayer", "IOR", 0));
        thicknessBox->setText(QApplication::translate("MaterialLayer", "Thickness", 0));
        importChannelButton->setText(QApplication::translate("MaterialLayer", "Import Image to Layer", 0));
        exportImageButton->setText(QApplication::translate("MaterialLayer", "Export Layer to Image", 0));
        exportFinalImageButton->setText(QApplication::translate("MaterialLayer", "Export Canvas to Image", 0));
    } // retranslateUi

};

namespace Ui {
    class MaterialLayer: public Ui_MaterialLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALLAYER_H
