/********************************************************************************
** Form generated from reading UI file 'dbwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBWINDOW_H
#define UI_DBWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBWindow
{
public:
    QWidget *centralwidget;
    QPushButton *addButton;
    QPushButton *removeButton_2;
    QTableView *tableView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DBWindow)
    {
        if (DBWindow->objectName().isEmpty())
            DBWindow->setObjectName(QString::fromUtf8("DBWindow"));
        DBWindow->resize(800, 600);
        centralwidget = new QWidget(DBWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(10, 530, 401, 24));
        removeButton_2 = new QPushButton(centralwidget);
        removeButton_2->setObjectName(QString::fromUtf8("removeButton_2"));
        removeButton_2->setGeometry(QRect(420, 530, 371, 24));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(10, 10, 781, 511));
        DBWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DBWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        DBWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DBWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        DBWindow->setStatusBar(statusbar);

        retranslateUi(DBWindow);

        QMetaObject::connectSlotsByName(DBWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DBWindow)
    {
        DBWindow->setWindowTitle(QCoreApplication::translate("DBWindow", "MainWindow", nullptr));
        addButton->setText(QCoreApplication::translate("DBWindow", "\320\264\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        removeButton_2->setText(QCoreApplication::translate("DBWindow", "\321\203\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBWindow: public Ui_DBWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBWINDOW_H
