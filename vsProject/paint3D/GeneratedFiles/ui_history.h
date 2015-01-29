/********************************************************************************
** Form generated from reading UI file 'history.ui'
**
** Created: Mon Sep 22 21:18:40 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORY_H
#define UI_HISTORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QUndoView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_History
{
public:
    QGridLayout *gridLayout;
    QUndoView *historyView;

    void setupUi(QWidget *History)
    {
        if (History->objectName().isEmpty())
            History->setObjectName(QString::fromUtf8("History"));
        History->resize(306, 235);
        gridLayout = new QGridLayout(History);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        historyView = new QUndoView(History);
        historyView->setObjectName(QString::fromUtf8("historyView"));

        gridLayout->addWidget(historyView, 0, 0, 1, 1);


        retranslateUi(History);

        QMetaObject::connectSlotsByName(History);
    } // setupUi

    void retranslateUi(QWidget *History)
    {
        History->setWindowTitle(QApplication::translate("History", "History", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class History: public Ui_History {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORY_H
