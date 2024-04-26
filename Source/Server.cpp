#include <QCoreApplication>
#include "Server.h"

Server::Server() {}
Server::~Server() {}

void Server::startServer()
{
    if (this->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "Listening";
    }
    else
    {
        qDebug() << "Not listening";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << socketDescriptor << " client connected";

    socket->write("You are connected");
    qDebug() << "Send client connection status - YES";
}

void Server::sockReady()
{


}

void Server::sockDisc()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer();

    return a.exec();
}
