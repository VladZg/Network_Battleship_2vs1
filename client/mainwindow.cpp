#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    is_logined = false;

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(readChannelFinished()), this, SLOT(sockDisc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()    // authorization button
{
    if (is_logined)
        return;

    qDebug() << "Connect button pushed";
    socket->connectToHost("127.0.0.1", 12345);

    QString login_entered = ui->plainTextEdit->toPlainText();

    if (!login_entered.isEmpty())
    {
        qDebug() << "trying to connect to server with login " << login_entered << "";

        socket->write(("AUTH " + login_entered).toUtf8());

        // TODO: check if login is already used
        // if (is_available) return;

        if (!socket->waitForReadyRead(5000)) return;    // if don't have server answer

        Data = socket->readAll();
        qDebug() << "server: " << Data;

        QString answer = QString::fromUtf8(Data).trimmed();
        if (answer.startsWith("AUTH_SUCCESS"))
        {
            is_logined = true;
            qDebug() << "loggining confirmed!";
        }

        // else: didn't connected
    }
}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}

void MainWindow::sockReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        qDebug() << "server: " << Data;
    }
}

void MainWindow::onDataRecieved()
{
//    Data = socket->readAll();
//    qDebug() << "Data: " << Data;
}

void MainWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{

}

