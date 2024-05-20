#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(quint16 port, QWidget *parent) :
    QMainWindow(parent),
    server_(Server(port)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startTimer(500);
//    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext& context, const QString& msg) {qDebug(msg.toUtf8()); this->ui->textBrowser->append(msg); });
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    server_.startServer(ui->textBrowser);
    ui->startButton->setDisabled(true);
}


void MainWindow::on_stopButton_clicked()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    server_.stopServer();
    QMessageBox::information(this, "info!", "Server stopped...");
    event->accept();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    ui->statusbar->showMessage("SERVER " + getServerStateStr());

    event->accept();
}

QString MainWindow::getServerStateStr()
{
    switch(server_.getServerState())
    {
        case Server::ServerState::ST_NSTARTED:
        {
            return QString("NOT STARTED");
        }
        case Server::ServerState::ST_STARTED:
        {
            return QString("STARTED");
        }
        case Server::ServerState::ST_STOPPED:
        {
            return QString("STOPPED");
        }
    }

    return "NONE";
}

void MainWindow::on_testDatabaseButton_clicked()
{
    server_.testDB();
//    qDebug() << "It does nothing yet. Its task for Timasok";
}

