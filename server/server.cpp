#include "server.h"

Server::Server(){}
Server::~Server(){}

void Server::startServer()
{
    if (this->listen(QHostAddress::Any, 12345))
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
}

bool Server::checkLoginRequest(QString& request)
{
    QString login = request.mid(5); // get login from the 1st line of request
    // TODO: Check login and send answer to the client
    if (true) //checkLogin(login)) {
    {
        qDebug() << "client \"" << login << "\" connected";
        return true;
    }

    return false;
}

void Server::sockReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        qDebug() << "client: " << Data;

        handleData();
    }
}

void Server::handleData()
{
    QString request = QString::fromUtf8(Data).trimmed();

//    qDebug() << "client: " << request;

    if (request.startsWith("AUTH"))
    {
        qDebug() << "AUTH!!!";

        // TODO: check if login is available
        if (checkLoginRequest(request))
        {
            socket->write("AUTH_SUCCESS\n");
            qDebug() << "Send client connection status - YES";
        }
        else
        {
            socket->write("AUTH_UNSUCCESS\n");
        }
    }

    // TODO: add more handlers
}

void Server::sockDisc()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}
