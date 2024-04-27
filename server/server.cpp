#include "server.h"

Server::Server(int port) : port_(port)
{

}

Server::~Server(){}

void Server::startServer()
{
    if (this->listen(QHostAddress::Any, port_))
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

    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(socket, SIGNAL(connected())   , this, SLOT(sockConnect())   );  // when socket connected, sockConnect slot runs
    connect(socket, SIGNAL(readyRead())   , this, SLOT(sockReady())     );  // when new network data comes, sockReady slot runs
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisconnect()));  // when socket disconnected, sockDisconnect slot runs
}

bool Server::checkLoginRequest(QString& request)
{
    QString login = request.mid(5); // get login from the 1st line of request
    // TODO: Check login and send answer to the client
    if (true) //checkLogin(login)) {
    {
        qDebug() << "client " << login << " connected";
        return true;
    }

    return false;
}

void Server::sockReady()
{
    data = socket->readAll();
    qDebug() << "client: " << data;
    handleData();
}

void Server::handleData()
{
    QString request = QString::fromUtf8(data).trimmed();
//    qDebug() << "client: " << request;

    if (request.startsWith("AUTH"))
    {
        qDebug() << "AUTH!!!";

        // TODO: check if login is available
        if (checkLoginRequest(request))
        {
            socket->write("AUTH_SUCCESS");
            qDebug() << "Send client connection status - YES";
        }
        else
        {
            socket->write("AUTH_UNSUCCESS");
        }
    }

    // TODO: add more handlers
}

void Server::sockConnect()
{
    qDebug() << "Connect";
}

void Server::sockDisconnect()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}

void Server::sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}
