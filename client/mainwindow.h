#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>

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

private:
    ClientState state_;
    QString ip_;
    int port_;
    QTcpSocket* socket_; // socket for interacting with server
    QByteArray data_;    // data from socket and for socket
    bool is_connected;
    bool is_logined;    // flag sets True after user is logined

public slots:
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);

private slots:
    void on_connectToServerButton_clicked();
    void on_loginLabel_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
