/********************************************************************************
** Form generated from reading UI file 'object.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransformEditor
{
public:
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *transLabel;
    QDoubleSpinBox *transXSpinBox;
    QDoubleSpinBox *transYSpinBox;
    QDoubleSpinBox *transZSpinBox;
    QLabel *rotLabel;
    QDoubleSpinBox *rotXSpinBox;
    QDoubleSpinBox *rotYSpinBox;
    QDoubleSpinBox *rotZSpinBox;
    QLabel *scaLabel;
    QDoubleSpinBox *scaleXSpinBox;
    QDoubleSpinBox *scaleYSpinBox;
    QDoubleSpinBox *scaleZSpinBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QScrollArea *TransformEditor)
    {
        if (TransformEditor->objectName().isEmpty())
            TransformEditor->setObjectName(QStringLiteral("TransformEditor"));
        TransformEditor->resize(319, 154);
        TransformEditor->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 324, 136));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        nameLabel = new QLabel(scrollAreaWidgetContents);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        nameEdit = new QLineEdit(scrollAreaWidgetContents);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));

        gridLayout->addWidget(nameEdit, 0, 1, 1, 2);

        transLabel = new QLabel(scrollAreaWidgetContents);
        transLabel->setObjectName(QStringLiteral("transLabel"));

        gridLayout->addWidget(transLabel, 1, 0, 1, 1);

        transXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transXSpinBox->setObjectName(QStringLiteral("transXSpinBox"));
        transXSpinBox->setMinimum(-10000);
        transXSpinBox->setMaximum(10000);
        transXSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transXSpinBox, 1, 1, 1, 1);

        transYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transYSpinBox->setObjectName(QStringLiteral("transYSpinBox"));
        transYSpinBox->setMinimum(-10000);
        transYSpinBox->setMaximum(10000);
        transYSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transYSpinBox, 1, 2, 1, 1);

        transZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transZSpinBox->setObjectName(QStringLiteral("transZSpinBox"));
        transZSpinBox->setMinimum(-10000);
        transZSpinBox->setMaximum(10000);
        transZSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transZSpinBox, 1, 3, 1, 1);

        rotLabel = new QLabel(scrollAreaWidgetContents);
        rotLabel->setObjectName(QStringLiteral("rotLabel"));

        gridLayout->addWidget(rotLabel, 2, 0, 1, 1);

        rotXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotXSpinBox->setObjectName(QStringLiteral("rotXSpinBox"));
        rotXSpinBox->setMinimum(-180);
        rotXSpinBox->setMaximum(180);

        gridLayout->addWidget(rotXSpinBox, 2, 1, 1, 1);

        rotYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotYSpinBox->setObjectName(QStringLiteral("rotYSpinBox"));
        rotYSpinBox->setMinimum(-180);
        rotYSpinBox->setMaximum(180);

        gridLayout->addWidget(rotYSpinBox, 2, 2, 1, 1);

        rotZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotZSpinBox->setObjectName(QStringLiteral("rotZSpinBox"));
        rotZSpinBox->setMinimum(-180);
        rotZSpinBox->setMaximum(180);

        gridLayout->addWidget(rotZSpinBox, 2, 3, 1, 1);

        scaLabel = new QLabel(scrollAreaWidgetContents);
        scaLabel->setObjectName(QStringLiteral("scaLabel"));

        gridLayout->addWidget(scaLabel, 3, 0, 1, 1);

        scaleXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleXSpinBox->setObjectName(QStringLiteral("scaleXSpinBox"));
        scaleXSpinBox->setMinimum(0.01);
        scaleXSpinBox->setMaximum(10000);
        scaleXSpinBox->setSingleStep(0.01);
        scaleXSpinBox->setValue(1);

        gridLayout->addWidget(scaleXSpinBox, 3, 1, 1, 1);

        scaleYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleYSpinBox->setObjectName(QStringLiteral("scaleYSpinBox"));
        scaleYSpinBox->setMinimum(0.01);
        scaleYSpinBox->setMaximum(10000);
        scaleYSpinBox->setSingleStep(0.01);
        scaleYSpinBox->setValue(1);

        gridLayout->addWidget(scaleYSpinBox, 3, 2, 1, 1);

        scaleZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleZSpinBox->setObjectName(QStringLiteral("scaleZSpinBox"));
        scaleZSpinBox->setMinimum(0.01);
        scaleZSpinBox->setMaximum(10000);
        scaleZSpinBox->setSingleStep(0.01);
        scaleZSpinBox->setValue(1);

        gridLayout->addWidget(scaleZSpinBox, 3, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 11, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 2, 1, 1);

        TransformEditor->setWidget(scrollAreaWidgetContents);

        retranslateUi(TransformEditor);

        QMetaObject::connectSlotsByName(TransformEditor);
    } // setupUi

    void retranslateUi(QScrollArea *TransformEditor)
    {
        TransformEditor->setWindowTitle(QApplication::translate("TransformEditor", "Object Transform", 0));
        nameLabel->setText(QApplication::translate("TransformEditor", "Name", 0));
        transLabel->setText(QApplication::translate("TransformEditor", "Translate", 0));
        rotLabel->setText(QApplication::translate("TransformEditor", "Rotate", 0));
        scaLabel->setText(QApplication::translate("TransformEditor", "Scale", 0));
    } // retranslateUi

};

namespace Ui {
    class TransformEditor: public Ui_TransformEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECT_H
