#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QListWidget>
#include <QTextBrowser>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum ClientState  // states of client
{
    ST_DISCONNECTED = 0,
    ST_CONNECTED       ,
    ST_AUTHORIZED      ,
    ST_READY           ,
    ST_WAITING_STEP    ,
    ST_MAKING_STEP     ,
    // more
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString ip, quint16 port, QWidget *parent = nullptr);
    ~MainWindow();
    void screenUpdate();
    void stateUpdate(ClientState new_state);
    void updateAll();

private:
    ClientState state_;
    QString ip_;
    int port_;
    QTcpSocket* socket_; // socket for interacting with server
    QByteArray data_;    // data from socket and for socket
    QString login_;

public:
    QStringList userLogins_;
    void handleData();
    void usersListUpdate();
    void sendMessage();
    void handleUsersListUpdate();
    void updateChats();

public slots:
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);

private slots:
    void on_connectToServerButton_clicked();
    void on_loginLabel_cursorPositionChanged(int arg1, int arg2);        
    void on_updateButton_clicked();
    void on_sendMessageButton_clicked();
    void on_messageRecieversOptionList_itemSelectionChanged();
    void createTextBrowser(QListWidgetItem* user);

private:
    QListWidget* receiversListWidget;    // user from the list who we want to send the message
    QStackedWidget* receiverBrowserStackedWidget;  // browser (chat) with this user
    QMap<QListWidgetItem*, QTextBrowser*> browserMap;   // map of the user logins and chats with them

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
