/********************************************************************************
** Form generated from reading UI file 'brushSetting.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRUSHSETTING_H
#define UI_BRUSHSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
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
            BrushSetting->setObjectName(QStringLiteral("BrushSetting"));
        BrushSetting->resize(329, 680);
        BrushSetting->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 327, 678));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        sizeLabel = new QLabel(groupBox);
        sizeLabel->setObjectName(QStringLiteral("sizeLabel"));

        gridLayout->addWidget(sizeLabel, 0, 0, 1, 1);

        sizeSpinBox = new SliderSpinnerGroup(groupBox);
        sizeSpinBox->setObjectName(QStringLiteral("sizeSpinBox"));
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
        hardnessLabel->setObjectName(QStringLiteral("hardnessLabel"));

        gridLayout->addWidget(hardnessLabel, 1, 0, 1, 1);

        hardnessSpinBox = new SliderSpinnerGroup(groupBox);
        hardnessSpinBox->setObjectName(QStringLiteral("hardnessSpinBox"));
        hardnessSpinBox->setMaximum(1);
        hardnessSpinBox->setValue(0);

        gridLayout->addWidget(hardnessSpinBox, 1, 1, 1, 1);

        angleLabel = new QLabel(groupBox);
        angleLabel->setObjectName(QStringLiteral("angleLabel"));

        gridLayout->addWidget(angleLabel, 2, 0, 1, 1);

        angleSpinBox = new SliderSpinnerGroup(groupBox);
        angleSpinBox->setObjectName(QStringLiteral("angleSpinBox"));
        angleSpinBox->setMaximum(360);
        angleSpinBox->setSingleStep(1);

        gridLayout->addWidget(angleSpinBox, 2, 1, 1, 1);

        ratioLabel = new QLabel(groupBox);
        ratioLabel->setObjectName(QStringLiteral("ratioLabel"));

        gridLayout->addWidget(ratioLabel, 3, 0, 1, 1);

        ratioSpinBox = new SliderSpinnerGroup(groupBox);
        ratioSpinBox->setObjectName(QStringLiteral("ratioSpinBox"));
        ratioSpinBox->setMaximum(5);
        ratioSpinBox->setMinimum(0.05);
        ratioSpinBox->setValue(1);

        gridLayout->addWidget(ratioSpinBox, 3, 1, 1, 1);

        intervalLabel = new QLabel(groupBox);
        intervalLabel->setObjectName(QStringLiteral("intervalLabel"));

        gridLayout->addWidget(intervalLabel, 4, 0, 1, 1);

        intervalSpinBox = new SliderSpinnerGroup(groupBox);
        intervalSpinBox->setObjectName(QStringLiteral("intervalSpinBox"));
        intervalSpinBox->setMaximum(2);
        intervalSpinBox->setMinimum(0.05);
        intervalSpinBox->setValue(0.06);

        gridLayout->addWidget(intervalSpinBox, 4, 1, 1, 1);

        offsetRangeLabel = new QLabel(groupBox);
        offsetRangeLabel->setObjectName(QStringLiteral("offsetRangeLabel"));

        gridLayout->addWidget(offsetRangeLabel, 5, 0, 1, 1);

        offsetRangeSpinBox = new SliderSpinnerGroup(groupBox);
        offsetRangeSpinBox->setObjectName(QStringLiteral("offsetRangeSpinBox"));
        offsetRangeSpinBox->setMaximum(10);

        gridLayout->addWidget(offsetRangeSpinBox, 5, 1, 1, 1);

        sizeJitterLabel = new QLabel(groupBox);
        sizeJitterLabel->setObjectName(QStringLiteral("sizeJitterLabel"));

        gridLayout->addWidget(sizeJitterLabel, 6, 0, 1, 1);

        sizeJitterSpinBox = new SliderSpinnerGroup(groupBox);
        sizeJitterSpinBox->setObjectName(QStringLiteral("sizeJitterSpinBox"));

        gridLayout->addWidget(sizeJitterSpinBox, 6, 1, 1, 1);

        ratioJitterLabel = new QLabel(groupBox);
        ratioJitterLabel->setObjectName(QStringLiteral("ratioJitterLabel"));

        gridLayout->addWidget(ratioJitterLabel, 7, 0, 1, 1);

        ratioJitterSpinBox = new SliderSpinnerGroup(groupBox);
        ratioJitterSpinBox->setObjectName(QStringLiteral("ratioJitterSpinBox"));

        gridLayout->addWidget(ratioJitterSpinBox, 7, 1, 1, 1);

        angleJitterLabel = new QLabel(groupBox);
        angleJitterLabel->setObjectName(QStringLiteral("angleJitterLabel"));

        gridLayout->addWidget(angleJitterLabel, 8, 0, 1, 1);

        angleJitterSpinBox = new SliderSpinnerGroup(groupBox);
        angleJitterSpinBox->setObjectName(QStringLiteral("angleJitterSpinBox"));

        gridLayout->addWidget(angleJitterSpinBox, 8, 1, 1, 1);

        resolutionRatioLabel = new QLabel(groupBox);
        resolutionRatioLabel->setObjectName(QStringLiteral("resolutionRatioLabel"));

        gridLayout->addWidget(resolutionRatioLabel, 9, 0, 1, 1);

        resolutionRatioSpinBox = new SliderSpinnerGroup(groupBox);
        resolutionRatioSpinBox->setObjectName(QStringLiteral("resolutionRatioSpinBox"));
        resolutionRatioSpinBox->setMaximum(110);
        resolutionRatioSpinBox->setMinimum(10);
        resolutionRatioSpinBox->setSingleStep(0.01);
        resolutionRatioSpinBox->setValue(50);

        gridLayout->addWidget(resolutionRatioSpinBox, 9, 1, 1, 1);

        nearPlaneLabel = new QLabel(groupBox);
        nearPlaneLabel->setObjectName(QStringLiteral("nearPlaneLabel"));

        gridLayout->addWidget(nearPlaneLabel, 10, 0, 1, 1);

        nearPlaneSpinBox = new SliderSpinnerGroup(groupBox);
        nearPlaneSpinBox->setObjectName(QStringLiteral("nearPlaneSpinBox"));
        nearPlaneSpinBox->setMaximum(0);
        nearPlaneSpinBox->setMinimum(-5);
        nearPlaneSpinBox->setValue(-1);

        gridLayout->addWidget(nearPlaneSpinBox, 10, 1, 1, 1);

        farPlaneLabel = new QLabel(groupBox);
        farPlaneLabel->setObjectName(QStringLiteral("farPlaneLabel"));

        gridLayout->addWidget(farPlaneLabel, 11, 0, 1, 1);

        farPlaneSpinBox = new SliderSpinnerGroup(groupBox);
        farPlaneSpinBox->setObjectName(QStringLiteral("farPlaneSpinBox"));
        farPlaneSpinBox->setMaximum(40);
        farPlaneSpinBox->setValue(10);

        gridLayout->addWidget(farPlaneSpinBox, 11, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(2);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        loadFromImageButton = new QPushButton(groupBox_2);
        loadFromImageButton->setObjectName(QStringLiteral("loadFromImageButton"));

        verticalLayout_5->addWidget(loadFromImageButton);

        loadFromAbrButton = new QPushButton(groupBox_2);
        loadFromAbrButton->setObjectName(QStringLiteral("loadFromAbrButton"));

        verticalLayout_5->addWidget(loadFromAbrButton);

        deleteStrokeButton = new QPushButton(groupBox_2);
        deleteStrokeButton->setObjectName(QStringLiteral("deleteStrokeButton"));

        verticalLayout_5->addWidget(deleteStrokeButton);


        verticalLayout_6->addLayout(verticalLayout_5);

        strokeList = new QListWidget(groupBox_2);
        strokeList->setObjectName(QStringLiteral("strokeList"));
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
        BrushSetting->setWindowTitle(QApplication::translate("BrushSetting", "Brush Settings", 0));
        groupBox->setTitle(QApplication::translate("BrushSetting", "Brush", 0));
        sizeLabel->setText(QApplication::translate("BrushSetting", "Size", 0));
        hardnessLabel->setText(QApplication::translate("BrushSetting", "Hardness", 0));
        angleLabel->setText(QApplication::translate("BrushSetting", "Angle", 0));
        ratioLabel->setText(QApplication::translate("BrushSetting", "Ratio", 0));
        intervalLabel->setText(QApplication::translate("BrushSetting", "Interval", 0));
        offsetRangeLabel->setText(QApplication::translate("BrushSetting", "Offset Range", 0));
        sizeJitterLabel->setText(QApplication::translate("BrushSetting", "Size Jitter", 0));
        ratioJitterLabel->setText(QApplication::translate("BrushSetting", "Ratio Jitter", 0));
        angleJitterLabel->setText(QApplication::translate("BrushSetting", "Angle Jitter", 0));
        resolutionRatioLabel->setText(QApplication::translate("BrushSetting", "Res. Ratio", 0));
        nearPlaneLabel->setText(QApplication::translate("BrushSetting", "Near Plane", 0));
        farPlaneLabel->setText(QApplication::translate("BrushSetting", "Far Plane", 0));
        groupBox_2->setTitle(QApplication::translate("BrushSetting", "Stroke", 0));
        loadFromImageButton->setText(QApplication::translate("BrushSetting", "load stroke from image", 0));
        loadFromAbrButton->setText(QApplication::translate("BrushSetting", "load stroke from Abr", 0));
        deleteStrokeButton->setText(QApplication::translate("BrushSetting", "delete stroke", 0));
    } // retranslateUi

};

namespace Ui {
    class BrushSetting: public Ui_BrushSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRUSHSETTING_H
