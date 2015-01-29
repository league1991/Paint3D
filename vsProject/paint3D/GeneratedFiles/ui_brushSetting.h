/********************************************************************************
** Form generated from reading UI file 'brushSetting.ui'
**
** Created: Mon Sep 22 21:18:40 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRUSHSETTING_H
#define UI_BRUSHSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "SliderSpinnerGroup.h"

QT_BEGIN_NAMESPACE

class Ui_BrushSetting
{
public:
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *sizeLabel;
    SliderSpinnerGroup *sizeSpinBox;
    QLabel *hardnessLabel;
    SliderSpinnerGroup *hardnessSpinBox;
    QLabel *angleLabel;
    SliderSpinnerGroup *angleSpinBox;
    QLabel *ratioLabel;
    SliderSpinnerGroup *ratioSpinBox;
    QLabel *intervalLabel;
    SliderSpinnerGroup *intervalSpinBox;
    QLabel *offsetRangeLabel;
    SliderSpinnerGroup *offsetRangeSpinBox;
    QLabel *sizeJitterLabel;
    SliderSpinnerGroup *sizeJitterSpinBox;
    QLabel *ratioJitterLabel;
    SliderSpinnerGroup *ratioJitterSpinBox;
    QLabel *angleJitterLabel;
    SliderSpinnerGroup *angleJitterSpinBox;
    QLabel *resolutionRatioLabel;
    SliderSpinnerGroup *resolutionRatioSpinBox;
    QLabel *nearPlaneLabel;
    SliderSpinnerGroup *nearPlaneSpinBox;
    QLabel *farPlaneLabel;
    SliderSpinnerGroup *farPlaneSpinBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QPushButton *loadFromImageButton;
    QPushButton *loadFromAbrButton;
    QPushButton *deleteStrokeButton;
    QListWidget *strokeList;

    void setupUi(QScrollArea *BrushSetting)
    {
        if (BrushSetting->objectName().isEmpty())
            BrushSetting->setObjectName(QString::fromUtf8("BrushSetting"));
        BrushSetting->resize(329, 680);
        BrushSetting->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 327, 678));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sizeLabel = new QLabel(groupBox);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));

        gridLayout->addWidget(sizeLabel, 0, 0, 1, 1);

        sizeSpinBox = new SliderSpinnerGroup(groupBox);
        sizeSpinBox->setObjectName(QString::fromUtf8("sizeSpinBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sizeSpinBox->sizePolicy().hasHeightForWidth());
        sizeSpinBox->setSizePolicy(sizePolicy);
        sizeSpinBox->setMaximum(5);
        sizeSpinBox->setMinimum(0.05);
        sizeSpinBox->setValue(1);

        gridLayout->addWidget(sizeSpinBox, 0, 1, 1, 1);

        hardnessLabel = new QLabel(groupBox);
        hardnessLabel->setObjectName(QString::fromUtf8("hardnessLabel"));

        gridLayout->addWidget(hardnessLabel, 1, 0, 1, 1);

        hardnessSpinBox = new SliderSpinnerGroup(groupBox);
        hardnessSpinBox->setObjectName(QString::fromUtf8("hardnessSpinBox"));
        hardnessSpinBox->setMaximum(1);
        hardnessSpinBox->setValue(0);

        gridLayout->addWidget(hardnessSpinBox, 1, 1, 1, 1);

        angleLabel = new QLabel(groupBox);
        angleLabel->setObjectName(QString::fromUtf8("angleLabel"));

        gridLayout->addWidget(angleLabel, 2, 0, 1, 1);

        angleSpinBox = new SliderSpinnerGroup(groupBox);
        angleSpinBox->setObjectName(QString::fromUtf8("angleSpinBox"));
        angleSpinBox->setMaximum(360);
        angleSpinBox->setSingleStep(1);

        gridLayout->addWidget(angleSpinBox, 2, 1, 1, 1);

        ratioLabel = new QLabel(groupBox);
        ratioLabel->setObjectName(QString::fromUtf8("ratioLabel"));

        gridLayout->addWidget(ratioLabel, 3, 0, 1, 1);

        ratioSpinBox = new SliderSpinnerGroup(groupBox);
        ratioSpinBox->setObjectName(QString::fromUtf8("ratioSpinBox"));
        ratioSpinBox->setMaximum(5);
        ratioSpinBox->setMinimum(0.05);
        ratioSpinBox->setValue(1);

        gridLayout->addWidget(ratioSpinBox, 3, 1, 1, 1);

        intervalLabel = new QLabel(groupBox);
        intervalLabel->setObjectName(QString::fromUtf8("intervalLabel"));

        gridLayout->addWidget(intervalLabel, 4, 0, 1, 1);

        intervalSpinBox = new SliderSpinnerGroup(groupBox);
        intervalSpinBox->setObjectName(QString::fromUtf8("intervalSpinBox"));
        intervalSpinBox->setMaximum(2);
        intervalSpinBox->setMinimum(0.05);
        intervalSpinBox->setValue(0.06);

        gridLayout->addWidget(intervalSpinBox, 4, 1, 1, 1);

        offsetRangeLabel = new QLabel(groupBox);
        offsetRangeLabel->setObjectName(QString::fromUtf8("offsetRangeLabel"));

        gridLayout->addWidget(offsetRangeLabel, 5, 0, 1, 1);

        offsetRangeSpinBox = new SliderSpinnerGroup(groupBox);
        offsetRangeSpinBox->setObjectName(QString::fromUtf8("offsetRangeSpinBox"));
        offsetRangeSpinBox->setMaximum(10);

        gridLayout->addWidget(offsetRangeSpinBox, 5, 1, 1, 1);

        sizeJitterLabel = new QLabel(groupBox);
        sizeJitterLabel->setObjectName(QString::fromUtf8("sizeJitterLabel"));

        gridLayout->addWidget(sizeJitterLabel, 6, 0, 1, 1);

        sizeJitterSpinBox = new SliderSpinnerGroup(groupBox);
        sizeJitterSpinBox->setObjectName(QString::fromUtf8("sizeJitterSpinBox"));

        gridLayout->addWidget(sizeJitterSpinBox, 6, 1, 1, 1);

        ratioJitterLabel = new QLabel(groupBox);
        ratioJitterLabel->setObjectName(QString::fromUtf8("ratioJitterLabel"));

        gridLayout->addWidget(ratioJitterLabel, 7, 0, 1, 1);

        ratioJitterSpinBox = new SliderSpinnerGroup(groupBox);
        ratioJitterSpinBox->setObjectName(QString::fromUtf8("ratioJitterSpinBox"));

        gridLayout->addWidget(ratioJitterSpinBox, 7, 1, 1, 1);

        angleJitterLabel = new QLabel(groupBox);
        angleJitterLabel->setObjectName(QString::fromUtf8("angleJitterLabel"));

        gridLayout->addWidget(angleJitterLabel, 8, 0, 1, 1);

        angleJitterSpinBox = new SliderSpinnerGroup(groupBox);
        angleJitterSpinBox->setObjectName(QString::fromUtf8("angleJitterSpinBox"));

        gridLayout->addWidget(angleJitterSpinBox, 8, 1, 1, 1);

        resolutionRatioLabel = new QLabel(groupBox);
        resolutionRatioLabel->setObjectName(QString::fromUtf8("resolutionRatioLabel"));

        gridLayout->addWidget(resolutionRatioLabel, 9, 0, 1, 1);

        resolutionRatioSpinBox = new SliderSpinnerGroup(groupBox);
        resolutionRatioSpinBox->setObjectName(QString::fromUtf8("resolutionRatioSpinBox"));
        resolutionRatioSpinBox->setMaximum(110);
        resolutionRatioSpinBox->setMinimum(10);
        resolutionRatioSpinBox->setSingleStep(0.01);
        resolutionRatioSpinBox->setValue(50);

        gridLayout->addWidget(resolutionRatioSpinBox, 9, 1, 1, 1);

        nearPlaneLabel = new QLabel(groupBox);
        nearPlaneLabel->setObjectName(QString::fromUtf8("nearPlaneLabel"));

        gridLayout->addWidget(nearPlaneLabel, 10, 0, 1, 1);

        nearPlaneSpinBox = new SliderSpinnerGroup(groupBox);
        nearPlaneSpinBox->setObjectName(QString::fromUtf8("nearPlaneSpinBox"));
        nearPlaneSpinBox->setMaximum(0);
        nearPlaneSpinBox->setMinimum(-5);
        nearPlaneSpinBox->setValue(-1);

        gridLayout->addWidget(nearPlaneSpinBox, 10, 1, 1, 1);

        farPlaneLabel = new QLabel(groupBox);
        farPlaneLabel->setObjectName(QString::fromUtf8("farPlaneLabel"));

        gridLayout->addWidget(farPlaneLabel, 11, 0, 1, 1);

        farPlaneSpinBox = new SliderSpinnerGroup(groupBox);
        farPlaneSpinBox->setObjectName(QString::fromUtf8("farPlaneSpinBox"));
        farPlaneSpinBox->setMaximum(40);
        farPlaneSpinBox->setValue(10);

        gridLayout->addWidget(farPlaneSpinBox, 11, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        loadFromImageButton = new QPushButton(groupBox_2);
        loadFromImageButton->setObjectName(QString::fromUtf8("loadFromImageButton"));

        verticalLayout_5->addWidget(loadFromImageButton);

        loadFromAbrButton = new QPushButton(groupBox_2);
        loadFromAbrButton->setObjectName(QString::fromUtf8("loadFromAbrButton"));

        verticalLayout_5->addWidget(loadFromAbrButton);

        deleteStrokeButton = new QPushButton(groupBox_2);
        deleteStrokeButton->setObjectName(QString::fromUtf8("deleteStrokeButton"));

        verticalLayout_5->addWidget(deleteStrokeButton);


        verticalLayout_6->addLayout(verticalLayout_5);

        strokeList = new QListWidget(groupBox_2);
        strokeList->setObjectName(QString::fromUtf8("strokeList"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(strokeList->sizePolicy().hasHeightForWidth());
        strokeList->setSizePolicy(sizePolicy1);
        strokeList->setMinimumSize(QSize(0, 0));
        strokeList->setProperty("isWrapping", QVariant(false));
        strokeList->setViewMode(QListView::ListMode);

        verticalLayout_6->addWidget(strokeList);


        verticalLayout->addWidget(groupBox_2);

        BrushSetting->setWidget(scrollAreaWidgetContents);

        retranslateUi(BrushSetting);

        QMetaObject::connectSlotsByName(BrushSetting);
    } // setupUi

    void retranslateUi(QScrollArea *BrushSetting)
    {
        BrushSetting->setWindowTitle(QApplication::translate("BrushSetting", "Brush Settings", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("BrushSetting", "Brush", 0, QApplication::UnicodeUTF8));
        sizeLabel->setText(QApplication::translate("BrushSetting", "Size", 0, QApplication::UnicodeUTF8));
        hardnessLabel->setText(QApplication::translate("BrushSetting", "Hardness", 0, QApplication::UnicodeUTF8));
        angleLabel->setText(QApplication::translate("BrushSetting", "Angle", 0, QApplication::UnicodeUTF8));
        ratioLabel->setText(QApplication::translate("BrushSetting", "Ratio", 0, QApplication::UnicodeUTF8));
        intervalLabel->setText(QApplication::translate("BrushSetting", "Interval", 0, QApplication::UnicodeUTF8));
        offsetRangeLabel->setText(QApplication::translate("BrushSetting", "Offset Range", 0, QApplication::UnicodeUTF8));
        sizeJitterLabel->setText(QApplication::translate("BrushSetting", "Size Jitter", 0, QApplication::UnicodeUTF8));
        ratioJitterLabel->setText(QApplication::translate("BrushSetting", "Ratio Jitter", 0, QApplication::UnicodeUTF8));
        angleJitterLabel->setText(QApplication::translate("BrushSetting", "Angle Jitter", 0, QApplication::UnicodeUTF8));
        resolutionRatioLabel->setText(QApplication::translate("BrushSetting", "Res. Ratio", 0, QApplication::UnicodeUTF8));
        nearPlaneLabel->setText(QApplication::translate("BrushSetting", "Near Plane", 0, QApplication::UnicodeUTF8));
        farPlaneLabel->setText(QApplication::translate("BrushSetting", "Far Plane", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("BrushSetting", "Stroke", 0, QApplication::UnicodeUTF8));
        loadFromImageButton->setText(QApplication::translate("BrushSetting", "load stroke from image", 0, QApplication::UnicodeUTF8));
        loadFromAbrButton->setText(QApplication::translate("BrushSetting", "load stroke from Abr", 0, QApplication::UnicodeUTF8));
        deleteStrokeButton->setText(QApplication::translate("BrushSetting", "delete stroke", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BrushSetting: public Ui_BrushSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRUSHSETTING_H
