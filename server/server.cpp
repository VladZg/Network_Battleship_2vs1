#include "server.h"
#include "client.h"

Server::Server(quint16 port) :
    port_(port)
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
    Client client;
//    client.socket_ = this->nextPendingConnection();
    client.socket_ = new QTcpSocket(this);
    client.socket_->setSocketDescriptor(socketDescriptor);
    client.status_ = Client::ST_CONNECTED;
//    client.playingWith = clients_.end();
    int clientId = client.socket_->socketDescriptor();

//    socket_ = new QTcpSocket(this);
//    socket_->setSocketDescriptor(socketDescriptor);

    connect(client.socket_, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(client.socket_, SIGNAL(connected())   , this, SLOT(on_sockConnect())   );  // when new socket connected, sockConnect slot runs
    connect(client.socket_, SIGNAL(readyRead())   , this, SLOT(on_receiveData())     );  // when new network data comes, sockReady slot runs
    connect(client.socket_, SIGNAL(disconnected()), this, SLOT(on_sockDisconnect()));  // when socket disconnected, sockDisconnect slot run

    clients_.insert(clientId, client);
}

void Server::on_receiveData()
{
    socket_ = (QTcpSocket*)sender();
    data_ = socket_->readAll();
    qDebug() << "client" << socket_->socketDescriptor() << ": " << data_;
    handleData(data_, socket_->socketDescriptor());
}

void Server::handleData(const QByteArray& data, int clientId)
{
    QString request = QString::fromUtf8(data).trimmed();
//    qDebug() << "client: " << request;

    ClientsIterator cit = clients_.find(clientId);

    if (request.startsWith("AUTH"))
    {
        QString login = request.mid(5); // get login from the 1st line of request
        if (checkLogin(login))   // check if login valid
        {
            logins_.insert(cit->socket_->socketDescriptor(), login);

            cit->socket_->write("AUTH_SUCCESS");
            qDebug() << "AUTH SUCCESS!!!";
            qDebug() << "Send client connection status - YES";
        }
        else
        {
            qDebug() << "AUTH UNSUCCESS... Already have " << login << " login";
            cit->socket_->write("AUTH_UNSUCCESS");
        }
    }

    // TODO: add more handlers
}

void Server::on_sockConnect()
{
    qDebug() << "Connect";
}

void Server::on_sockDisconnect()
{
    qDebug() << "Disconnect";
//    socket_->deleteLater();
}

void Server::on_sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}

bool Server::checkLogin(QString& login) // check if login available
{
    if (!logins_. values().contains(login))
    {
        qDebug() << "client " << login << " connected";
        return true;
    }

    return false;
}
