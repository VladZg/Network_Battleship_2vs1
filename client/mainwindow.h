#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTcpSocket* socket; // socket for interacting with server
    QByteArray Data;    // data from socket and for socket
    bool is_logined;    // flag sets True after user is logined

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void onDataRecieved();
    void on_pushButton_clicked();
    void on_plainTextEdit_blockCountChanged(int newBlockCount);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
