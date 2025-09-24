/********************************************************************************
** Form generated from reading UI file 'workplace.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKPLACE_H
#define UI_WORKPLACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorkPlace
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WorkPlace)
    {
        if (WorkPlace->objectName().isEmpty())
            WorkPlace->setObjectName("WorkPlace");
        WorkPlace->resize(1100, 600);
        WorkPlace->setMinimumSize(QSize(1100, 600));
        WorkPlace->setMaximumSize(QSize(1100, 600));
        centralwidget = new QWidget(WorkPlace);
        centralwidget->setObjectName("centralwidget");
        WorkPlace->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WorkPlace);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1100, 21));
        WorkPlace->setMenuBar(menubar);
        statusbar = new QStatusBar(WorkPlace);
        statusbar->setObjectName("statusbar");
        WorkPlace->setStatusBar(statusbar);

        retranslateUi(WorkPlace);

        QMetaObject::connectSlotsByName(WorkPlace);
    } // setupUi

    void retranslateUi(QMainWindow *WorkPlace)
    {
        WorkPlace->setWindowTitle(QCoreApplication::translate("WorkPlace", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WorkPlace: public Ui_WorkPlace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKPLACE_H
