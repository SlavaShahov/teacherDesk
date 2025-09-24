/********************************************************************************
** Form generated from reading UI file 'coachpage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COACHPAGE_H
#define UI_COACHPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CoachPage
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CoachPage)
    {
        if (CoachPage->objectName().isEmpty())
            CoachPage->setObjectName("CoachPage");
        CoachPage->resize(1100, 600);
        CoachPage->setMinimumSize(QSize(1100, 600));
        CoachPage->setMaximumSize(QSize(1100, 600));
        centralwidget = new QWidget(CoachPage);
        centralwidget->setObjectName("centralwidget");
        CoachPage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CoachPage);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 21));
        CoachPage->setMenuBar(menubar);
        statusbar = new QStatusBar(CoachPage);
        statusbar->setObjectName("statusbar");
        CoachPage->setStatusBar(statusbar);

        retranslateUi(CoachPage);

        QMetaObject::connectSlotsByName(CoachPage);
    } // setupUi

    void retranslateUi(QMainWindow *CoachPage)
    {
        CoachPage->setWindowTitle(QCoreApplication::translate("CoachPage", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CoachPage: public Ui_CoachPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COACHPAGE_H
