/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLineEdit *loginLabel;
    QPushButton *connectToServerButton;
    QPushButton *updateButton;
    QPushButton *sendMessageButton;
    QTextEdit *messageEdit;
    QWidget *messageRecieversOptionsListWidget;
    QListWidget *messageRecieversOptionList;
    QWidget *chatWidget;
    QLabel *fieldsLabel;
    QLabel *myGameLoginLabel;
    QLabel *enemyGameLoginLabel;
    QToolButton *gameExitButton;
    QPushButton *generateFieldButton;
    QPushButton *applyFieldButton;
    QWidget *formLayoutWidget_2;
    QFormLayout *debugButtonsLayout;
    QPushButton *checkButton;
    QPushButton *switchButton;
    QPushButton *generateButton;
    QPushButton *startButton;
    QPushButton *clearButton;
    QLabel *isCorrectLabel;
    QMenuBar *menubar;
    QMenu *usersList;
    QMenu *readyPlayersList;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(896, 500);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 161, 57));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetMinimumSize);
        formLayout->setLabelAlignment(Qt::AlignCenter);
        formLayout->setFormAlignment(Qt::AlignCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        loginLabel = new QLineEdit(formLayoutWidget);
        loginLabel->setObjectName(QString::fromUtf8("loginLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Unispace"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(true);
        font1.setWeight(9);
        loginLabel->setFont(font1);
        loginLabel->setStyleSheet(QString::fromUtf8("font: 75 italic 10pt \"Unispace\";"));
        loginLabel->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::FieldRole, loginLabel);

        connectToServerButton = new QPushButton(formLayoutWidget);
        connectToServerButton->setObjectName(QString::fromUtf8("connectToServerButton"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Ubuntu Condensed"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setItalic(true);
        font2.setWeight(9);
        connectToServerButton->setFont(font2);
        connectToServerButton->setStyleSheet(QString::fromUtf8("font: 75 italic 10pt \"Ubuntu Condensed\";"));

        formLayout->setWidget(1, QFormLayout::FieldRole, connectToServerButton);

        updateButton = new QPushButton(centralwidget);
        updateButton->setObjectName(QString::fromUtf8("updateButton"));
        updateButton->setGeometry(QRect(40, 60, 80, 24));
        sendMessageButton = new QPushButton(centralwidget);
        sendMessageButton->setObjectName(QString::fromUtf8("sendMessageButton"));
        sendMessageButton->setGeometry(QRect(40, 420, 80, 24));
        messageEdit = new QTextEdit(centralwidget);
        messageEdit->setObjectName(QString::fromUtf8("messageEdit"));
        messageEdit->setGeometry(QRect(10, 380, 151, 31));
        messageEdit->setLayoutDirection(Qt::RightToLeft);
        messageRecieversOptionsListWidget = new QWidget(centralwidget);
        messageRecieversOptionsListWidget->setObjectName(QString::fromUtf8("messageRecieversOptionsListWidget"));
        messageRecieversOptionsListWidget->setGeometry(QRect(10, 90, 151, 101));
        messageRecieversOptionList = new QListWidget(messageRecieversOptionsListWidget);
        messageRecieversOptionList->setObjectName(QString::fromUtf8("messageRecieversOptionList"));
        messageRecieversOptionList->setGeometry(QRect(0, 0, 151, 101));
        chatWidget = new QWidget(centralwidget);
        chatWidget->setObjectName(QString::fromUtf8("chatWidget"));
        chatWidget->setGeometry(QRect(10, 200, 151, 171));
        fieldsLabel = new QLabel(centralwidget);
        fieldsLabel->setObjectName(QString::fromUtf8("fieldsLabel"));
        fieldsLabel->setGeometry(QRect(200, 20, 641, 421));
        myGameLoginLabel = new QLabel(centralwidget);
        myGameLoginLabel->setObjectName(QString::fromUtf8("myGameLoginLabel"));
        myGameLoginLabel->setGeometry(QRect(310, 30, 81, 16));
        QFont font3;
        font3.setPointSize(15);
        font3.setBold(true);
        font3.setWeight(75);
        myGameLoginLabel->setFont(font3);
        enemyGameLoginLabel = new QLabel(centralwidget);
        enemyGameLoginLabel->setObjectName(QString::fromUtf8("enemyGameLoginLabel"));
        enemyGameLoginLabel->setGeometry(QRect(620, 30, 81, 16));
        enemyGameLoginLabel->setFont(font3);
        gameExitButton = new QToolButton(centralwidget);
        gameExitButton->setObjectName(QString::fromUtf8("gameExitButton"));
        gameExitButton->setGeometry(QRect(850, 20, 24, 23));
        gameExitButton->setStyleSheet(QString::fromUtf8("background-image: url(:/images/killed.png);"));
        generateFieldButton = new QPushButton(centralwidget);
        generateFieldButton->setObjectName(QString::fromUtf8("generateFieldButton"));
        generateFieldButton->setGeometry(QRect(230, 60, 111, 24));
        applyFieldButton = new QPushButton(centralwidget);
        applyFieldButton->setObjectName(QString::fromUtf8("applyFieldButton"));
        applyFieldButton->setGeometry(QRect(460, 420, 111, 24));
        formLayoutWidget_2 = new QWidget(centralwidget);
        formLayoutWidget_2->setObjectName(QString::fromUtf8("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(220, 370, 201, 91));
        debugButtonsLayout = new QFormLayout(formLayoutWidget_2);
        debugButtonsLayout->setObjectName(QString::fromUtf8("debugButtonsLayout"));
        debugButtonsLayout->setContentsMargins(0, 0, 0, 0);
        checkButton = new QPushButton(formLayoutWidget_2);
        checkButton->setObjectName(QString::fromUtf8("checkButton"));

        debugButtonsLayout->setWidget(0, QFormLayout::FieldRole, checkButton);

        switchButton = new QPushButton(formLayoutWidget_2);
        switchButton->setObjectName(QString::fromUtf8("switchButton"));

        debugButtonsLayout->setWidget(1, QFormLayout::SpanningRole, switchButton);

        generateButton = new QPushButton(formLayoutWidget_2);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));
        QFont font4;
        font4.setPointSize(8);
        font4.setBold(true);
        font4.setWeight(75);
        generateButton->setFont(font4);

        debugButtonsLayout->setWidget(2, QFormLayout::LabelRole, generateButton);

        startButton = new QPushButton(formLayoutWidget_2);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        debugButtonsLayout->setWidget(0, QFormLayout::LabelRole, startButton);

        clearButton = new QPushButton(formLayoutWidget_2);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        debugButtonsLayout->setWidget(2, QFormLayout::FieldRole, clearButton);

        isCorrectLabel = new QLabel(centralwidget);
        isCorrectLabel->setObjectName(QString::fromUtf8("isCorrectLabel"));
        isCorrectLabel->setGeometry(QRect(460, 370, 111, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 896, 21));
        usersList = new QMenu(menubar);
        usersList->setObjectName(QString::fromUtf8("usersList"));
        readyPlayersList = new QMenu(menubar);
        readyPlayersList->setObjectName(QString::fromUtf8("readyPlayersList"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font5.setPointSize(10);
        font5.setBold(false);
        font5.setItalic(false);
        font5.setWeight(50);
        statusbar->setFont(font5);
        statusbar->setStyleSheet(QString::fromUtf8("font: 10pt \"Ubuntu Mono\";\n"
"color: rgb(220, 138, 221);"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(usersList->menuAction());
        menubar->addAction(readyPlayersList->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Network_Battleship_2vs1", nullptr));
        loginLabel->setText(QString());
        loginLabel->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\273\320\276\320\263\320\270\320\275", nullptr));
        connectToServerButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217 \321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\276\320\274", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        sendMessageButton->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", nullptr));
        messageEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\265", nullptr));
        fieldsLabel->setText(QString());
        myGameLoginLabel->setText(QString());
        enemyGameLoginLabel->setText(QString());
        gameExitButton->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        generateFieldButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        applyFieldButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\264\320\270\321\202\321\214", nullptr));
        checkButton->setText(QCoreApplication::translate("MainWindow", "check", nullptr));
        switchButton->setText(QCoreApplication::translate("MainWindow", "\321\201\320\274\320\265\320\275\320\270\321\202\321\214 \321\205\320\276\320\264", nullptr));
        generateButton->setText(QCoreApplication::translate("MainWindow", "\321\201\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "start", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "\320\276\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        isCorrectLabel->setText(QString());
        usersList->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", nullptr));
        readyPlayersList->setTitle(QCoreApplication::translate("MainWindow", "\320\230\320\263\321\200\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
