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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString ip, int port, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString ip_;
    int port_;
    QTcpSocket* socket; // socket for interacting with server
    QByteArray data;    // data from socket and for socket
    bool is_logined;    // flag sets True after user is logined

public slots:
    void sockConnect();
    void sockReady();
    void sockDisconnect();
    void sockError(QAbstractSocket::SocketError error);

private slots:
    void on_pushButton_clicked();
    void on_plainTextEdit_blockCountChanged(int newBlockCount);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
