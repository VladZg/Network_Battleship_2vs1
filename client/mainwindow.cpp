#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QString ip, quint16 port, QWidget *parent)
    : QMainWindow(parent)
    , ip_(ip)
    , port_(port)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket_ = new QTcpSocket(this);
    is_logined = false;
    is_connected = false;

    stateUpdate(ST_DISCONNECTED);

    connect(socket_, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(socket_, SIGNAL(connected())   , this, SLOT(on_sockConnect())   );  // when socket connected, sockConnect slot runs
    connect(socket_, SIGNAL(readyRead())   , this, SLOT(on_receiveData())     );  // when new network data comes, sockReady slot runs
    connect(socket_, SIGNAL(disconnected()), this, SLOT(on_sockDisconnect()));  // when socket disconnected, sockDisconnect slot runs
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stateUpdate(ClientState new_state)
{
    switch (new_state)
    {
        case ST_DISCONNECTED:
        {
            ui->statusbar->showMessage("status: ST_DISCONNECTED");
            break;
        }
        case ST_CONNECTED:
        {
            ui->statusbar->showMessage("status: ST_CONNECTED");
            break;
        }
        case ST_AUTHORIZED:
        {
            ui->statusbar->showMessage("status: ST_AUTHORIZED");
            break;
        }
        case ST_MAKING_STEP:
        {
            ui->statusbar->showMessage("status: ST_MAKING_STEP");
            break;
        }
        case ST_WAITING_STEP:
        {
            ui->statusbar->showMessage("status: ST_WAITING_STEP");
            break;
        }
        case ST_READY:
        {
            ui->statusbar->showMessage("status: ST_READY");
            break;
        }
    }

    state_ = new_state;
}

void MainWindow::screenUpdate()
{

}

void MainWindow::on_connectToServerButton_clicked()    // authorization button
{
    if (is_logined)
        return;

    if (!is_connected)
    {
        qDebug() << "Connect button pushed";
        socket_->connectToHost(ip_, port_);

        if (!socket_->waitForConnected(500))
        {
            qDebug() << "Cannot connect";
            QMessageBox::warning(this, "ERROR!", "Cannot connect user to server...");
            return;
        }

        is_connected = true;
//        qDebug() << "Connected";
    }

    stateUpdate(ST_CONNECTED);

    QString login_entered = ui->loginLabel->text();

    if (!login_entered.isEmpty())
    {
        qDebug() << "trying to connect to server with login " << login_entered << "";

        socket_->write(("AUTH " + login_entered).toUtf8());

        // TODO: check if login is already used
        // if (is_available) return;

        if (!socket_->waitForReadyRead(2500)) // if server don't answer > 2.5 sec
        {
            qDebug() << "Server don't answer";
            return;
        }

        QString answer = QString::fromUtf8(data_).trimmed();
        if (answer.startsWith("AUTH_SUCCESS"))
        {
            is_logined = true;
            stateUpdate(ST_AUTHORIZED);

            ui->loginLabel->setStyleSheet("background-color: green;\n color: white;");

            qDebug() << "loggining confirmed!";
        }

         else // didn't connected
        {
            ui->loginLabel->setStyleSheet("background-color: red;\n color: white;");
            QMessageBox::warning(this, "ERROR!", "Login is already in use");
        }
    }
}

void MainWindow::on_sockConnect()
{

}

void MainWindow::on_sockDisconnect()
{
    socket_->deleteLater();
}

void MainWindow::on_sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}

void MainWindow::on_receiveData()
{
//    socket->waitForReadyRead(500);
    data_ = socket_->readAll();
    qDebug() << "server: " << data_;
}

void MainWindow::on_loginLabel_cursorPositionChanged(int , int )
{

}

