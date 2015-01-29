/********************************************************************************
** Form generated from reading UI file 'object.ui'
**
** Created: Mon Sep 22 21:18:39 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

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
            TransformEditor->setObjectName(QString::fromUtf8("TransformEditor"));
        TransformEditor->resize(319, 154);
        TransformEditor->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 324, 136));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        nameLabel = new QLabel(scrollAreaWidgetContents);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        nameEdit = new QLineEdit(scrollAreaWidgetContents);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        gridLayout->addWidget(nameEdit, 0, 1, 1, 2);

        transLabel = new QLabel(scrollAreaWidgetContents);
        transLabel->setObjectName(QString::fromUtf8("transLabel"));

        gridLayout->addWidget(transLabel, 1, 0, 1, 1);

        transXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transXSpinBox->setObjectName(QString::fromUtf8("transXSpinBox"));
        transXSpinBox->setMinimum(-10000);
        transXSpinBox->setMaximum(10000);
        transXSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transXSpinBox, 1, 1, 1, 1);

        transYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transYSpinBox->setObjectName(QString::fromUtf8("transYSpinBox"));
        transYSpinBox->setMinimum(-10000);
        transYSpinBox->setMaximum(10000);
        transYSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transYSpinBox, 1, 2, 1, 1);

        transZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        transZSpinBox->setObjectName(QString::fromUtf8("transZSpinBox"));
        transZSpinBox->setMinimum(-10000);
        transZSpinBox->setMaximum(10000);
        transZSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(transZSpinBox, 1, 3, 1, 1);

        rotLabel = new QLabel(scrollAreaWidgetContents);
        rotLabel->setObjectName(QString::fromUtf8("rotLabel"));

        gridLayout->addWidget(rotLabel, 2, 0, 1, 1);

        rotXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotXSpinBox->setObjectName(QString::fromUtf8("rotXSpinBox"));
        rotXSpinBox->setMinimum(-180);
        rotXSpinBox->setMaximum(180);

        gridLayout->addWidget(rotXSpinBox, 2, 1, 1, 1);

        rotYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotYSpinBox->setObjectName(QString::fromUtf8("rotYSpinBox"));
        rotYSpinBox->setMinimum(-180);
        rotYSpinBox->setMaximum(180);

        gridLayout->addWidget(rotYSpinBox, 2, 2, 1, 1);

        rotZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        rotZSpinBox->setObjectName(QString::fromUtf8("rotZSpinBox"));
        rotZSpinBox->setMinimum(-180);
        rotZSpinBox->setMaximum(180);

        gridLayout->addWidget(rotZSpinBox, 2, 3, 1, 1);

        scaLabel = new QLabel(scrollAreaWidgetContents);
        scaLabel->setObjectName(QString::fromUtf8("scaLabel"));

        gridLayout->addWidget(scaLabel, 3, 0, 1, 1);

        scaleXSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleXSpinBox->setObjectName(QString::fromUtf8("scaleXSpinBox"));
        scaleXSpinBox->setMinimum(0.01);
        scaleXSpinBox->setMaximum(10000);
        scaleXSpinBox->setSingleStep(0.01);
        scaleXSpinBox->setValue(1);

        gridLayout->addWidget(scaleXSpinBox, 3, 1, 1, 1);

        scaleYSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleYSpinBox->setObjectName(QString::fromUtf8("scaleYSpinBox"));
        scaleYSpinBox->setMinimum(0.01);
        scaleYSpinBox->setMaximum(10000);
        scaleYSpinBox->setSingleStep(0.01);
        scaleYSpinBox->setValue(1);

        gridLayout->addWidget(scaleYSpinBox, 3, 2, 1, 1);

        scaleZSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        scaleZSpinBox->setObjectName(QString::fromUtf8("scaleZSpinBox"));
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
        TransformEditor->setWindowTitle(QApplication::translate("TransformEditor", "Object Transform", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QApplication::translate("TransformEditor", "Name", 0, QApplication::UnicodeUTF8));
        transLabel->setText(QApplication::translate("TransformEditor", "Translate", 0, QApplication::UnicodeUTF8));
        rotLabel->setText(QApplication::translate("TransformEditor", "Rotate", 0, QApplication::UnicodeUTF8));
        scaLabel->setText(QApplication::translate("TransformEditor", "Scale", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TransformEditor: public Ui_TransformEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECT_H
