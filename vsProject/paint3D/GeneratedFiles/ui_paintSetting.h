/********************************************************************************
** Form generated from reading UI file 'paintSetting.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTSETTING_H
#define UI_PAINTSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "SliderSpinnerGroup.h"
#include "blendcurvegraphwidget.h"
#include "colorselectbutton.h"

QT_BEGIN_NAMESPACE

class Ui_PaintSetting
{
public:
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *modeGroup;
    QHBoxLayout *horizontalLayout;
    QRadioButton *brushModeRadio;
    QRadioButton *eraserModeRadio;
    QGroupBox *brushParamGroup;
    QGridLayout *gridLayout;
    QRadioButton *pureColorRadio;
    QLabel *colorLabel;
    QCheckBox *affectColor;
    ColorSelectButton *colorButton;
    QLabel *colorTexLabel;
    QToolButton *colorTexButton;
    QToolButton *clearColorTexButton;
    QLabel *opacityLabel;
    SliderSpinnerGroup *opacitySpinBox;
    QLabel *reflLevelLabel;
    QCheckBox *affectReflLevel;
    SliderSpinnerGroup *reflLevelSpinBox;
    QLabel *reflTexLabel;
    QToolButton *reflTexButton;
    QToolButton *clearReflTexButton;
    QLabel *reflGlossnessLabel;
    QCheckBox *affectGlossness;
    SliderSpinnerGroup *reflGlossnessSpinBox;
    QLabel *glosTexLabel;
    QToolButton *glossnessTexButton;
    QToolButton *clearGlossnessTexButton;
    QLabel *refrLevelLabel;
    QCheckBox *affectRefrLevel;
    SliderSpinnerGroup *refrLevelSpinBox;
    QLabel *refrTexLabel;
    QToolButton *refrTexButton;
    QToolButton *clearRefrTexButton;
    QLabel *refrGlossnessLabel;
    QCheckBox *affectIOR;
    SliderSpinnerGroup *refrGlossnessSpinBox;
    QLabel *iorTexLabel;
    QToolButton *iorTexButton;
    QToolButton *clearIorTexButton;
    QLabel *thicknessLabel;
    QCheckBox *affectThickness;
    SliderSpinnerGroup *thicknessSpinBox;
    QLabel *thicknessTexLabel;
    QToolButton *thicknessTexButton;
    QToolButton *clearThicknessTexButton;
    QRadioButton *colorPickerRadio;
    QPushButton *selectPickerButton;
    QLabel *pickerOpacityLabel;
    QCheckBox *attachToCameraBox;
    QSpacerItem *horizontalSpacer;
    QLabel *xOffsetLabel;
    SliderSpinnerGroup *xOffsetSpinBox;
    QSpacerItem *horizontalSpacer_2;
    QLabel *yOffsetLabel;
    SliderSpinnerGroup *yOffsetSpinBox;
    QSpacerItem *horizontalSpacer_4;
    QLabel *zOffsetLabel;
    SliderSpinnerGroup *zOffsetSpinBox;
    QSpacerItem *horizontalSpacer_3;
    QLabel *rotateLabel;
    SliderSpinnerGroup *rotateSpinBox;
    QSpacerItem *horizontalSpacer_5;
    QLabel *xScaleLabel;
    SliderSpinnerGroup *xScaleSpinBox;
    QSpacerItem *horizontalSpacer_6;
    QLabel *yScaleLabel;
    SliderSpinnerGroup *yScaleSpinBox;
    SliderSpinnerGroup *pickerOpacitySpinBox;
    QGroupBox *normalControlGroup;
    QGridLayout *gridLayout_2;
    QLabel *normalV0Label;
    SliderSpinnerGroup *normalV0Spinner;
    BlendCurveGraphWidget *normalCurveView;
    QLabel *normalV1Label;
    SliderSpinnerGroup *normalV1Spinner;
    QLabel *normalWeight0Label;
    SliderSpinnerGroup *normalW0Spinner;
    QLabel *normalWeight1Label;
    SliderSpinnerGroup *normalW1Spinner;
    QGroupBox *depthControlGroup;
    QGridLayout *gridLayout_3;
    QLabel *depthTypeLabel;
    QComboBox *depthTypeBox;
    QLabel *depthV0Label;
    SliderSpinnerGroup *depthV0Spinner;
    BlendCurveGraphWidget *depthCurveView;
    QLabel *depthV1Label;
    SliderSpinnerGroup *depthV1Spinner;
    QLabel *depthWeight0Label;
    SliderSpinnerGroup *depthW0Spinner;
    QLabel *depthWeight1Label;
    SliderSpinnerGroup *depthW1Spinner;
    QSpacerItem *verticalSpacer;

    void setupUi(QScrollArea *PaintSetting)
    {
        if (PaintSetting->objectName().isEmpty())
            PaintSetting->setObjectName(QStringLiteral("PaintSetting"));
        PaintSetting->resize(425, 760);
        PaintSetting->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 408, 866));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        modeGroup = new QGroupBox(scrollAreaWidgetContents);
        modeGroup->setObjectName(QStringLiteral("modeGroup"));
        horizontalLayout = new QHBoxLayout(modeGroup);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        brushModeRadio = new QRadioButton(modeGroup);
        brushModeRadio->setObjectName(QStringLiteral("brushModeRadio"));
        brushModeRadio->setChecked(true);

        horizontalLayout->addWidget(brushModeRadio);

        eraserModeRadio = new QRadioButton(modeGroup);
        eraserModeRadio->setObjectName(QStringLiteral("eraserModeRadio"));

        horizontalLayout->addWidget(eraserModeRadio);


        verticalLayout->addWidget(modeGroup);

        brushParamGroup = new QGroupBox(scrollAreaWidgetContents);
        brushParamGroup->setObjectName(QStringLiteral("brushParamGroup"));
        brushParamGroup->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(brushParamGroup->sizePolicy().hasHeightForWidth());
        brushParamGroup->setSizePolicy(sizePolicy);
        brushParamGroup->setFlat(false);
        brushParamGroup->setCheckable(false);
        gridLayout = new QGridLayout(brushParamGroup);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pureColorRadio = new QRadioButton(brushParamGroup);
        pureColorRadio->setObjectName(QStringLiteral("pureColorRadio"));
        pureColorRadio->setChecked(true);

        gridLayout->addWidget(pureColorRadio, 0, 0, 1, 2);

        colorLabel = new QLabel(brushParamGroup);
        colorLabel->setObjectName(QStringLiteral("colorLabel"));
        colorLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(colorLabel, 1, 0, 1, 2);

        affectColor = new QCheckBox(brushParamGroup);
        affectColor->setObjectName(QStringLiteral("affectColor"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(affectColor->sizePolicy().hasHeightForWidth());
        affectColor->setSizePolicy(sizePolicy1);
        affectColor->setChecked(true);

        gridLayout->addWidget(affectColor, 1, 2, 1, 1);

        colorButton = new ColorSelectButton(brushParamGroup);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(colorButton->sizePolicy().hasHeightForWidth());
        colorButton->setSizePolicy(sizePolicy2);
        colorButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(colorButton, 1, 3, 1, 1);

        colorTexLabel = new QLabel(brushParamGroup);
        colorTexLabel->setObjectName(QStringLiteral("colorTexLabel"));
        colorTexLabel->setMaximumSize(QSize(22, 22));
        colorTexLabel->setAutoFillBackground(false);
        colorTexLabel->setFrameShape(QFrame::NoFrame);
        colorTexLabel->setFrameShadow(QFrame::Plain);
        colorTexLabel->setLineWidth(1);
        colorTexLabel->setMidLineWidth(0);
        colorTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        colorTexLabel->setScaledContents(true);

        gridLayout->addWidget(colorTexLabel, 1, 4, 1, 1);

        colorTexButton = new QToolButton(brushParamGroup);
        colorTexButton->setObjectName(QStringLiteral("colorTexButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/paint3D/Resources/plus.png"), QSize(), QIcon::Normal, QIcon::Off);
        colorTexButton->setIcon(icon);

        gridLayout->addWidget(colorTexButton, 1, 5, 1, 1);

        clearColorTexButton = new QToolButton(brushParamGroup);
        clearColorTexButton->setObjectName(QStringLiteral("clearColorTexButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/paint3D/Resources/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearColorTexButton->setIcon(icon1);

        gridLayout->addWidget(clearColorTexButton, 1, 6, 1, 1);

        opacityLabel = new QLabel(brushParamGroup);
        opacityLabel->setObjectName(QStringLiteral("opacityLabel"));
        opacityLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(opacityLabel, 2, 0, 1, 2);

        opacitySpinBox = new SliderSpinnerGroup(brushParamGroup);
        opacitySpinBox->setObjectName(QStringLiteral("opacitySpinBox"));
        opacitySpinBox->setMaximum(1);
        opacitySpinBox->setMinimum(0);
        opacitySpinBox->setValue(1);

        gridLayout->addWidget(opacitySpinBox, 2, 3, 1, 1);

        reflLevelLabel = new QLabel(brushParamGroup);
        reflLevelLabel->setObjectName(QStringLiteral("reflLevelLabel"));
        reflLevelLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(reflLevelLabel, 3, 0, 1, 2);

        affectReflLevel = new QCheckBox(brushParamGroup);
        affectReflLevel->setObjectName(QStringLiteral("affectReflLevel"));
        sizePolicy1.setHeightForWidth(affectReflLevel->sizePolicy().hasHeightForWidth());
        affectReflLevel->setSizePolicy(sizePolicy1);
        affectReflLevel->setChecked(true);

        gridLayout->addWidget(affectReflLevel, 3, 2, 1, 1);

        reflLevelSpinBox = new SliderSpinnerGroup(brushParamGroup);
        reflLevelSpinBox->setObjectName(QStringLiteral("reflLevelSpinBox"));

        gridLayout->addWidget(reflLevelSpinBox, 3, 3, 1, 1);

        reflTexLabel = new QLabel(brushParamGroup);
        reflTexLabel->setObjectName(QStringLiteral("reflTexLabel"));
        reflTexLabel->setMaximumSize(QSize(22, 22));
        reflTexLabel->setFrameShape(QFrame::NoFrame);
        reflTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        reflTexLabel->setScaledContents(true);

        gridLayout->addWidget(reflTexLabel, 3, 4, 1, 1);

        reflTexButton = new QToolButton(brushParamGroup);
        reflTexButton->setObjectName(QStringLiteral("reflTexButton"));
        reflTexButton->setIcon(icon);

        gridLayout->addWidget(reflTexButton, 3, 5, 1, 1);

        clearReflTexButton = new QToolButton(brushParamGroup);
        clearReflTexButton->setObjectName(QStringLiteral("clearReflTexButton"));
        clearReflTexButton->setIcon(icon1);

        gridLayout->addWidget(clearReflTexButton, 3, 6, 1, 1);

        reflGlossnessLabel = new QLabel(brushParamGroup);
        reflGlossnessLabel->setObjectName(QStringLiteral("reflGlossnessLabel"));
        reflGlossnessLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(reflGlossnessLabel, 4, 0, 1, 2);

        affectGlossness = new QCheckBox(brushParamGroup);
        affectGlossness->setObjectName(QStringLiteral("affectGlossness"));
        sizePolicy1.setHeightForWidth(affectGlossness->sizePolicy().hasHeightForWidth());
        affectGlossness->setSizePolicy(sizePolicy1);
        affectGlossness->setChecked(true);

        gridLayout->addWidget(affectGlossness, 4, 2, 1, 1);

        reflGlossnessSpinBox = new SliderSpinnerGroup(brushParamGroup);
        reflGlossnessSpinBox->setObjectName(QStringLiteral("reflGlossnessSpinBox"));
        reflGlossnessSpinBox->setInputMethodHints(Qt::ImhNone);

        gridLayout->addWidget(reflGlossnessSpinBox, 4, 3, 1, 1);

        glosTexLabel = new QLabel(brushParamGroup);
        glosTexLabel->setObjectName(QStringLiteral("glosTexLabel"));
        glosTexLabel->setMaximumSize(QSize(22, 22));
        glosTexLabel->setFrameShape(QFrame::NoFrame);
        glosTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        glosTexLabel->setScaledContents(true);

        gridLayout->addWidget(glosTexLabel, 4, 4, 1, 1);

        glossnessTexButton = new QToolButton(brushParamGroup);
        glossnessTexButton->setObjectName(QStringLiteral("glossnessTexButton"));
        glossnessTexButton->setIcon(icon);

        gridLayout->addWidget(glossnessTexButton, 4, 5, 1, 1);

        clearGlossnessTexButton = new QToolButton(brushParamGroup);
        clearGlossnessTexButton->setObjectName(QStringLiteral("clearGlossnessTexButton"));
        clearGlossnessTexButton->setIcon(icon1);

        gridLayout->addWidget(clearGlossnessTexButton, 4, 6, 1, 1);

        refrLevelLabel = new QLabel(brushParamGroup);
        refrLevelLabel->setObjectName(QStringLiteral("refrLevelLabel"));
        refrLevelLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(refrLevelLabel, 5, 0, 1, 2);

        affectRefrLevel = new QCheckBox(brushParamGroup);
        affectRefrLevel->setObjectName(QStringLiteral("affectRefrLevel"));
        sizePolicy1.setHeightForWidth(affectRefrLevel->sizePolicy().hasHeightForWidth());
        affectRefrLevel->setSizePolicy(sizePolicy1);
        affectRefrLevel->setChecked(true);

        gridLayout->addWidget(affectRefrLevel, 5, 2, 1, 1);

        refrLevelSpinBox = new SliderSpinnerGroup(brushParamGroup);
        refrLevelSpinBox->setObjectName(QStringLiteral("refrLevelSpinBox"));
        refrLevelSpinBox->setValue(1);

        gridLayout->addWidget(refrLevelSpinBox, 5, 3, 1, 1);

        refrTexLabel = new QLabel(brushParamGroup);
        refrTexLabel->setObjectName(QStringLiteral("refrTexLabel"));
        refrTexLabel->setMaximumSize(QSize(22, 22));
        refrTexLabel->setFrameShape(QFrame::NoFrame);
        refrTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        refrTexLabel->setScaledContents(true);

        gridLayout->addWidget(refrTexLabel, 5, 4, 1, 1);

        refrTexButton = new QToolButton(brushParamGroup);
        refrTexButton->setObjectName(QStringLiteral("refrTexButton"));
        refrTexButton->setIcon(icon);

        gridLayout->addWidget(refrTexButton, 5, 5, 1, 1);

        clearRefrTexButton = new QToolButton(brushParamGroup);
        clearRefrTexButton->setObjectName(QStringLiteral("clearRefrTexButton"));
        clearRefrTexButton->setIcon(icon1);

        gridLayout->addWidget(clearRefrTexButton, 5, 6, 1, 1);

        refrGlossnessLabel = new QLabel(brushParamGroup);
        refrGlossnessLabel->setObjectName(QStringLiteral("refrGlossnessLabel"));
        refrGlossnessLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(refrGlossnessLabel, 6, 0, 1, 2);

        affectIOR = new QCheckBox(brushParamGroup);
        affectIOR->setObjectName(QStringLiteral("affectIOR"));
        sizePolicy1.setHeightForWidth(affectIOR->sizePolicy().hasHeightForWidth());
        affectIOR->setSizePolicy(sizePolicy1);
        affectIOR->setChecked(true);

        gridLayout->addWidget(affectIOR, 6, 2, 1, 1);

        refrGlossnessSpinBox = new SliderSpinnerGroup(brushParamGroup);
        refrGlossnessSpinBox->setObjectName(QStringLiteral("refrGlossnessSpinBox"));

        gridLayout->addWidget(refrGlossnessSpinBox, 6, 3, 1, 1);

        iorTexLabel = new QLabel(brushParamGroup);
        iorTexLabel->setObjectName(QStringLiteral("iorTexLabel"));
        iorTexLabel->setMaximumSize(QSize(22, 22));
        iorTexLabel->setFrameShape(QFrame::NoFrame);
        iorTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        iorTexLabel->setScaledContents(true);

        gridLayout->addWidget(iorTexLabel, 6, 4, 1, 1);

        iorTexButton = new QToolButton(brushParamGroup);
        iorTexButton->setObjectName(QStringLiteral("iorTexButton"));
        iorTexButton->setIcon(icon);

        gridLayout->addWidget(iorTexButton, 6, 5, 1, 1);

        clearIorTexButton = new QToolButton(brushParamGroup);
        clearIorTexButton->setObjectName(QStringLiteral("clearIorTexButton"));
        clearIorTexButton->setIcon(icon1);

        gridLayout->addWidget(clearIorTexButton, 6, 6, 1, 1);

        thicknessLabel = new QLabel(brushParamGroup);
        thicknessLabel->setObjectName(QStringLiteral("thicknessLabel"));
        thicknessLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(thicknessLabel, 7, 0, 1, 2);

        affectThickness = new QCheckBox(brushParamGroup);
        affectThickness->setObjectName(QStringLiteral("affectThickness"));
        sizePolicy1.setHeightForWidth(affectThickness->sizePolicy().hasHeightForWidth());
        affectThickness->setSizePolicy(sizePolicy1);
        affectThickness->setChecked(true);

        gridLayout->addWidget(affectThickness, 7, 2, 1, 1);

        thicknessSpinBox = new SliderSpinnerGroup(brushParamGroup);
        thicknessSpinBox->setObjectName(QStringLiteral("thicknessSpinBox"));

        gridLayout->addWidget(thicknessSpinBox, 7, 3, 1, 1);

        thicknessTexLabel = new QLabel(brushParamGroup);
        thicknessTexLabel->setObjectName(QStringLiteral("thicknessTexLabel"));
        thicknessTexLabel->setMaximumSize(QSize(22, 22));
        thicknessTexLabel->setFrameShape(QFrame::NoFrame);
        thicknessTexLabel->setPixmap(QPixmap(QString::fromUtf8(":/paint3D/Resources/null.png")));
        thicknessTexLabel->setScaledContents(true);

        gridLayout->addWidget(thicknessTexLabel, 7, 4, 1, 1);

        thicknessTexButton = new QToolButton(brushParamGroup);
        thicknessTexButton->setObjectName(QStringLiteral("thicknessTexButton"));
        thicknessTexButton->setIcon(icon);

        gridLayout->addWidget(thicknessTexButton, 7, 5, 1, 1);

        clearThicknessTexButton = new QToolButton(brushParamGroup);
        clearThicknessTexButton->setObjectName(QStringLiteral("clearThicknessTexButton"));
        clearThicknessTexButton->setIcon(icon1);

        gridLayout->addWidget(clearThicknessTexButton, 7, 6, 1, 1);

        colorPickerRadio = new QRadioButton(brushParamGroup);
        colorPickerRadio->setObjectName(QStringLiteral("colorPickerRadio"));

        gridLayout->addWidget(colorPickerRadio, 8, 0, 1, 2);

        selectPickerButton = new QPushButton(brushParamGroup);
        selectPickerButton->setObjectName(QStringLiteral("selectPickerButton"));
        selectPickerButton->setEnabled(false);
        selectPickerButton->setCheckable(true);
        selectPickerButton->setChecked(false);

        gridLayout->addWidget(selectPickerButton, 9, 0, 1, 4);

        pickerOpacityLabel = new QLabel(brushParamGroup);
        pickerOpacityLabel->setObjectName(QStringLiteral("pickerOpacityLabel"));
        pickerOpacityLabel->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(pickerOpacityLabel, 10, 0, 1, 2);

        attachToCameraBox = new QCheckBox(brushParamGroup);
        attachToCameraBox->setObjectName(QStringLiteral("attachToCameraBox"));
        attachToCameraBox->setEnabled(false);

        gridLayout->addWidget(attachToCameraBox, 11, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(57, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 12, 0, 1, 1);

        xOffsetLabel = new QLabel(brushParamGroup);
        xOffsetLabel->setObjectName(QStringLiteral("xOffsetLabel"));

        gridLayout->addWidget(xOffsetLabel, 12, 1, 1, 1);

        xOffsetSpinBox = new SliderSpinnerGroup(brushParamGroup);
        xOffsetSpinBox->setObjectName(QStringLiteral("xOffsetSpinBox"));
        xOffsetSpinBox->setEnabled(false);
        xOffsetSpinBox->setMinimum(-1);

        gridLayout->addWidget(xOffsetSpinBox, 12, 2, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(57, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 13, 0, 1, 1);

        yOffsetLabel = new QLabel(brushParamGroup);
        yOffsetLabel->setObjectName(QStringLiteral("yOffsetLabel"));

        gridLayout->addWidget(yOffsetLabel, 13, 1, 1, 1);

        yOffsetSpinBox = new SliderSpinnerGroup(brushParamGroup);
        yOffsetSpinBox->setObjectName(QStringLiteral("yOffsetSpinBox"));
        yOffsetSpinBox->setEnabled(false);
        yOffsetSpinBox->setMinimum(-1);

        gridLayout->addWidget(yOffsetSpinBox, 13, 2, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(57, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 14, 0, 1, 1);

        zOffsetLabel = new QLabel(brushParamGroup);
        zOffsetLabel->setObjectName(QStringLiteral("zOffsetLabel"));

        gridLayout->addWidget(zOffsetLabel, 14, 1, 1, 1);

        zOffsetSpinBox = new SliderSpinnerGroup(brushParamGroup);
        zOffsetSpinBox->setObjectName(QStringLiteral("zOffsetSpinBox"));
        zOffsetSpinBox->setEnabled(false);
        zOffsetSpinBox->setMinimum(0);

        gridLayout->addWidget(zOffsetSpinBox, 14, 2, 1, 2);

        horizontalSpacer_3 = new QSpacerItem(57, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 15, 0, 1, 1);

        rotateLabel = new QLabel(brushParamGroup);
        rotateLabel->setObjectName(QStringLiteral("rotateLabel"));

        gridLayout->addWidget(rotateLabel, 15, 1, 1, 1);

        rotateSpinBox = new SliderSpinnerGroup(brushParamGroup);
        rotateSpinBox->setObjectName(QStringLiteral("rotateSpinBox"));
        rotateSpinBox->setEnabled(false);
        rotateSpinBox->setMaximum(360);

        gridLayout->addWidget(rotateSpinBox, 15, 2, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(57, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 16, 0, 1, 1);

        xScaleLabel = new QLabel(brushParamGroup);
        xScaleLabel->setObjectName(QStringLiteral("xScaleLabel"));

        gridLayout->addWidget(xScaleLabel, 16, 1, 1, 1);

        xScaleSpinBox = new SliderSpinnerGroup(brushParamGroup);
        xScaleSpinBox->setObjectName(QStringLiteral("xScaleSpinBox"));
        xScaleSpinBox->setEnabled(false);
        xScaleSpinBox->setMaximum(1);
        xScaleSpinBox->setMinimum(-1);
        xScaleSpinBox->setValue(1);

        gridLayout->addWidget(xScaleSpinBox, 16, 2, 1, 2);

        horizontalSpacer_6 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 17, 0, 1, 1);

        yScaleLabel = new QLabel(brushParamGroup);
        yScaleLabel->setObjectName(QStringLiteral("yScaleLabel"));

        gridLayout->addWidget(yScaleLabel, 17, 1, 1, 1);

        yScaleSpinBox = new SliderSpinnerGroup(brushParamGroup);
        yScaleSpinBox->setObjectName(QStringLiteral("yScaleSpinBox"));
        yScaleSpinBox->setEnabled(false);
        yScaleSpinBox->setMaximum(1);
        yScaleSpinBox->setMinimum(-1);
        yScaleSpinBox->setValue(1);

        gridLayout->addWidget(yScaleSpinBox, 17, 2, 1, 2);

        pickerOpacitySpinBox = new SliderSpinnerGroup(brushParamGroup);
        pickerOpacitySpinBox->setObjectName(QStringLiteral("pickerOpacitySpinBox"));
        pickerOpacitySpinBox->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pickerOpacitySpinBox->sizePolicy().hasHeightForWidth());
        pickerOpacitySpinBox->setSizePolicy(sizePolicy3);
        pickerOpacitySpinBox->setValue(0.5);

        gridLayout->addWidget(pickerOpacitySpinBox, 10, 2, 1, 2);


        verticalLayout->addWidget(brushParamGroup);

        normalControlGroup = new QGroupBox(scrollAreaWidgetContents);
        normalControlGroup->setObjectName(QStringLiteral("normalControlGroup"));
        gridLayout_2 = new QGridLayout(normalControlGroup);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        normalV0Label = new QLabel(normalControlGroup);
        normalV0Label->setObjectName(QStringLiteral("normalV0Label"));

        gridLayout_2->addWidget(normalV0Label, 0, 0, 1, 1);

        normalV0Spinner = new SliderSpinnerGroup(normalControlGroup);
        normalV0Spinner->setObjectName(QStringLiteral("normalV0Spinner"));
        normalV0Spinner->setValue(1);

        gridLayout_2->addWidget(normalV0Spinner, 0, 1, 1, 1);

        normalCurveView = new BlendCurveGraphWidget(normalControlGroup);
        normalCurveView->setObjectName(QStringLiteral("normalCurveView"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(normalCurveView->sizePolicy().hasHeightForWidth());
        normalCurveView->setSizePolicy(sizePolicy4);
        normalCurveView->setMinimumSize(QSize(100, 0));

        gridLayout_2->addWidget(normalCurveView, 0, 2, 4, 1);

        normalV1Label = new QLabel(normalControlGroup);
        normalV1Label->setObjectName(QStringLiteral("normalV1Label"));

        gridLayout_2->addWidget(normalV1Label, 1, 0, 1, 1);

        normalV1Spinner = new SliderSpinnerGroup(normalControlGroup);
        normalV1Spinner->setObjectName(QStringLiteral("normalV1Spinner"));
        normalV1Spinner->setValue(1);

        gridLayout_2->addWidget(normalV1Spinner, 1, 1, 1, 1);

        normalWeight0Label = new QLabel(normalControlGroup);
        normalWeight0Label->setObjectName(QStringLiteral("normalWeight0Label"));

        gridLayout_2->addWidget(normalWeight0Label, 2, 0, 1, 1);

        normalW0Spinner = new SliderSpinnerGroup(normalControlGroup);
        normalW0Spinner->setObjectName(QStringLiteral("normalW0Spinner"));
        normalW0Spinner->setMaximum(5);
        normalW0Spinner->setMinimum(0.01);
        normalW0Spinner->setValue(1);

        gridLayout_2->addWidget(normalW0Spinner, 2, 1, 1, 1);

        normalWeight1Label = new QLabel(normalControlGroup);
        normalWeight1Label->setObjectName(QStringLiteral("normalWeight1Label"));

        gridLayout_2->addWidget(normalWeight1Label, 3, 0, 1, 1);

        normalW1Spinner = new SliderSpinnerGroup(normalControlGroup);
        normalW1Spinner->setObjectName(QStringLiteral("normalW1Spinner"));
        normalW1Spinner->setMaximum(5);
        normalW1Spinner->setMinimum(0.01);
        normalW1Spinner->setValue(1);

        gridLayout_2->addWidget(normalW1Spinner, 3, 1, 1, 1);


        verticalLayout->addWidget(normalControlGroup);

        depthControlGroup = new QGroupBox(scrollAreaWidgetContents);
        depthControlGroup->setObjectName(QStringLiteral("depthControlGroup"));
        gridLayout_3 = new QGridLayout(depthControlGroup);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        depthTypeLabel = new QLabel(depthControlGroup);
        depthTypeLabel->setObjectName(QStringLiteral("depthTypeLabel"));

        gridLayout_3->addWidget(depthTypeLabel, 0, 0, 1, 1);

        depthTypeBox = new QComboBox(depthControlGroup);
        depthTypeBox->setObjectName(QStringLiteral("depthTypeBox"));

        gridLayout_3->addWidget(depthTypeBox, 0, 1, 1, 1);

        depthV0Label = new QLabel(depthControlGroup);
        depthV0Label->setObjectName(QStringLiteral("depthV0Label"));

        gridLayout_3->addWidget(depthV0Label, 1, 0, 1, 1);

        depthV0Spinner = new SliderSpinnerGroup(depthControlGroup);
        depthV0Spinner->setObjectName(QStringLiteral("depthV0Spinner"));
        depthV0Spinner->setValue(1);

        gridLayout_3->addWidget(depthV0Spinner, 1, 1, 1, 1);

        depthCurveView = new BlendCurveGraphWidget(depthControlGroup);
        depthCurveView->setObjectName(QStringLiteral("depthCurveView"));
        sizePolicy4.setHeightForWidth(depthCurveView->sizePolicy().hasHeightForWidth());
        depthCurveView->setSizePolicy(sizePolicy4);
        depthCurveView->setMinimumSize(QSize(100, 0));

        gridLayout_3->addWidget(depthCurveView, 1, 2, 4, 1);

        depthV1Label = new QLabel(depthControlGroup);
        depthV1Label->setObjectName(QStringLiteral("depthV1Label"));

        gridLayout_3->addWidget(depthV1Label, 2, 0, 1, 1);

        depthV1Spinner = new SliderSpinnerGroup(depthControlGroup);
        depthV1Spinner->setObjectName(QStringLiteral("depthV1Spinner"));
        depthV1Spinner->setValue(1);

        gridLayout_3->addWidget(depthV1Spinner, 2, 1, 1, 1);

        depthWeight0Label = new QLabel(depthControlGroup);
        depthWeight0Label->setObjectName(QStringLiteral("depthWeight0Label"));

        gridLayout_3->addWidget(depthWeight0Label, 3, 0, 1, 1);

        depthW0Spinner = new SliderSpinnerGroup(depthControlGroup);
        depthW0Spinner->setObjectName(QStringLiteral("depthW0Spinner"));
        depthW0Spinner->setMaximum(5);
        depthW0Spinner->setMinimum(0.01);
        depthW0Spinner->setValue(1);

        gridLayout_3->addWidget(depthW0Spinner, 3, 1, 1, 1);

        depthWeight1Label = new QLabel(depthControlGroup);
        depthWeight1Label->setObjectName(QStringLiteral("depthWeight1Label"));

        gridLayout_3->addWidget(depthWeight1Label, 4, 0, 1, 1);

        depthW1Spinner = new SliderSpinnerGroup(depthControlGroup);
        depthW1Spinner->setObjectName(QStringLiteral("depthW1Spinner"));
        depthW1Spinner->setMaximum(5);
        depthW1Spinner->setMinimum(0.01);
        depthW1Spinner->setValue(1);

        gridLayout_3->addWidget(depthW1Spinner, 4, 1, 1, 1);


        verticalLayout->addWidget(depthControlGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        PaintSetting->setWidget(scrollAreaWidgetContents);

        retranslateUi(PaintSetting);
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), colorButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), opacitySpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), reflLevelSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), reflGlossnessSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), refrLevelSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), refrGlossnessSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), thicknessSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(colorPickerRadio, SIGNAL(toggled(bool)), pickerOpacitySpinBox, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), colorTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearColorTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), reflTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearReflTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), glossnessTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearGlossnessTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), refrTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearRefrTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), iorTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearIorTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), thicknessTexButton, SLOT(setEnabled(bool)));
        QObject::connect(pureColorRadio, SIGNAL(toggled(bool)), clearThicknessTexButton, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), xOffsetSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), yOffsetSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), rotateSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(colorPickerRadio, SIGNAL(toggled(bool)), selectPickerButton, SLOT(setEnabled(bool)));
        QObject::connect(colorPickerRadio, SIGNAL(toggled(bool)), pickerOpacitySpinBox, SLOT(setEnabled(bool)));
        QObject::connect(colorPickerRadio, SIGNAL(toggled(bool)), attachToCameraBox, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), zOffsetSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), xScaleSpinBox, SLOT(setEnabled(bool)));
        QObject::connect(attachToCameraBox, SIGNAL(toggled(bool)), yScaleSpinBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(PaintSetting);
    } // setupUi

    void retranslateUi(QScrollArea *PaintSetting)
    {
        PaintSetting->setWindowTitle(QApplication::translate("PaintSetting", "Paint Settings", 0));
        modeGroup->setTitle(QApplication::translate("PaintSetting", "Brush Mode", 0));
        brushModeRadio->setText(QApplication::translate("PaintSetting", "Brush", 0));
        eraserModeRadio->setText(QApplication::translate("PaintSetting", "Eraser", 0));
        brushParamGroup->setTitle(QApplication::translate("PaintSetting", "Brush Parameters", 0));
        pureColorRadio->setText(QApplication::translate("PaintSetting", "Use Pure Paint", 0));
        colorLabel->setText(QApplication::translate("PaintSetting", "Color", 0));
        affectColor->setText(QString());
        colorButton->setText(QApplication::translate("PaintSetting", "...", 0));
        colorTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        colorTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        colorTexButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        clearColorTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearColorTexButton->setText(QString());
        opacityLabel->setText(QApplication::translate("PaintSetting", "Flow", 0));
        reflLevelLabel->setText(QApplication::translate("PaintSetting", "Reflection Level", 0));
        affectReflLevel->setText(QString());
        reflTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        reflTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        reflTexButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        clearReflTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearReflTexButton->setText(QString());
        reflGlossnessLabel->setText(QApplication::translate("PaintSetting", "Glossness", 0));
        affectGlossness->setText(QString());
        glosTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        glossnessTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        glossnessTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
#ifndef QT_NO_TOOLTIP
        clearGlossnessTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearGlossnessTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
        refrLevelLabel->setText(QApplication::translate("PaintSetting", "Opacity", 0));
        affectRefrLevel->setText(QString());
        refrTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        refrTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        refrTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
#ifndef QT_NO_TOOLTIP
        clearRefrTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearRefrTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
        refrGlossnessLabel->setText(QApplication::translate("PaintSetting", "IOR", 0));
        affectIOR->setText(QString());
        iorTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        iorTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        iorTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
#ifndef QT_NO_TOOLTIP
        clearIorTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearIorTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
        thicknessLabel->setText(QApplication::translate("PaintSetting", "Thickness", 0));
        affectThickness->setText(QString());
        thicknessTexLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        thicknessTexButton->setToolTip(QApplication::translate("PaintSetting", "Add Texture", 0));
#endif // QT_NO_TOOLTIP
        thicknessTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
#ifndef QT_NO_TOOLTIP
        clearThicknessTexButton->setToolTip(QApplication::translate("PaintSetting", "Clear Texture", 0));
#endif // QT_NO_TOOLTIP
        clearThicknessTexButton->setText(QApplication::translate("PaintSetting", "PushButton", 0));
        colorPickerRadio->setText(QApplication::translate("PaintSetting", "Use Paint Picker", 0));
        selectPickerButton->setText(QApplication::translate("PaintSetting", "Select Picker From Scene..", 0));
        pickerOpacityLabel->setText(QApplication::translate("PaintSetting", "Opacity", 0));
        attachToCameraBox->setText(QApplication::translate("PaintSetting", "Attach To Camera", 0));
        xOffsetLabel->setText(QApplication::translate("PaintSetting", "X Offset", 0));
        yOffsetLabel->setText(QApplication::translate("PaintSetting", "Y Offset", 0));
        zOffsetLabel->setText(QApplication::translate("PaintSetting", "Z Offset", 0));
        rotateLabel->setText(QApplication::translate("PaintSetting", "Rotate", 0));
        xScaleLabel->setText(QApplication::translate("PaintSetting", "X Scale", 0));
        yScaleLabel->setText(QApplication::translate("PaintSetting", "Y Scale", 0));
        normalControlGroup->setTitle(QApplication::translate("PaintSetting", "Normal Control", 0));
        normalV0Label->setText(QApplication::translate("PaintSetting", "V0", 0));
        normalV1Label->setText(QApplication::translate("PaintSetting", "V1", 0));
        normalWeight0Label->setText(QApplication::translate("PaintSetting", "Weight 0", 0));
        normalWeight1Label->setText(QApplication::translate("PaintSetting", "Weight 1", 0));
        depthControlGroup->setTitle(QApplication::translate("PaintSetting", "Depth Control", 0));
        depthTypeLabel->setText(QApplication::translate("PaintSetting", "Depth Type", 0));
        depthTypeBox->clear();
        depthTypeBox->insertItems(0, QStringList()
         << QApplication::translate("PaintSetting", "Surface Thickness", 0)
         << QApplication::translate("PaintSetting", "Geometry Depth", 0)
        );
        depthV0Label->setText(QApplication::translate("PaintSetting", "V0", 0));
        depthV1Label->setText(QApplication::translate("PaintSetting", "V1", 0));
        depthWeight0Label->setText(QApplication::translate("PaintSetting", "Weight 0", 0));
        depthWeight1Label->setText(QApplication::translate("PaintSetting", "Weight 1", 0));
    } // retranslateUi

};

namespace Ui {
    class PaintSetting: public Ui_PaintSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTSETTING_H
