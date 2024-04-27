#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString ip, int port, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ip_(ip)
    , port_(port)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    is_logined = false;

    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(socket, SIGNAL(connected())   , this, SLOT(sockConnect())   );  // when socket connected, sockConnect slot runs
    connect(socket, SIGNAL(readyRead())   , this, SLOT(sockReady())     );  // when new network data comes, sockReady slot runs
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisconnect()));  // when socket disconnected, sockDisconnect slot runs
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
    socket->connectToHost(ip_, port_);

    if (!socket->waitForConnected(500))
    {
        qDebug() << "Cannot connect";
        return;
    }

    QString login_entered = ui->plainTextEdit->toPlainText();

    if (!login_entered.isEmpty())
    {
        qDebug() << "trying to connect to server with login " << login_entered << "";

        socket->write(("AUTH " + login_entered).toUtf8());

        // TODO: check if login is already used
        // if (is_available) return;

        if (!socket->waitForReadyRead(2500)) // if server don't answer > 2.5 sec
        {
            qDebug() << "Server don't answer";
            return;
        }

        QString answer = QString::fromUtf8(data).trimmed();
        if (answer.startsWith("AUTH_SUCCESS"))
        {
            is_logined = true;
            qDebug() << "loggining confirmed!";
        }

        // else: didn't connected
    }
}

void MainWindow::sockConnect()
{

}

void MainWindow::sockDisconnect()
{
    socket->deleteLater();
}

void MainWindow::sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}

void MainWindow::sockReady()
{
//    socket->waitForReadyRead(500);
    data = socket->readAll();
    qDebug() << "server: " << data;
}

void MainWindow::on_plainTextEdit_blockCountChanged(int)
{

}

