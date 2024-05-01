#include "server.h"
#include "client.h"
#include "field.h"
#include <QString>
#include <QStringList>
#include <QMessageBox>

//static int NUM_IND = 0;
#define PRINT(msg) { qDebug() << msg; browser->append(msg); }

Server::Server(quint16 port) :
    port_(port)
{

}

Server::Server(const Server& other) :
    port_(other.port_)
{

}

Server& Server::operator=(const Server& other)
{
    if (this == &other)
        return *this;

    port_ = other.port_;
    updateState(ST_NSTARTED);

    return *this;
}

Server::~Server()
{

}

void Server::startServer(QTextBrowser* textBrowser)
{
    if (!this->listen(QHostAddress::Any, port_))
    {
        qDebug() << "Not listening";
        QMessageBox::warning(textBrowser, "ERROR!", "Cannot start server on port " + QString::number(port_) + "... Try again!");
        return;
    }

    browser = textBrowser;

    PRINT("Listening")
    updateState(ST_STARTED);

    textBrowser->setStyleSheet("background-color: rgb(255, 255, 255, 5);"\
                               "background-image: url(:/images/images/background.jpg);"\
                               "color: rgb(255, 255, 177);");

    timerId_ = startTimer(DEFAULT_SEARCH_INTERVAL);
}

void Server::stopServer()
{
    killTimer(timerId_);

    sendMessageToAll("STOP:");
    PRINT("server: STOP: to all clients")
    updateState(ST_STOPPED);

    // TODO: finish the function
}

void Server::updateState(ServerState state)
{
    state_ = state;
}

Server::ServerState Server::getServerState()
{
    return state_;
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
    PRINT("client" + QString::number(socket_->socketDescriptor()) + ": " + data_)
    handleData(data_, socket_->socketDescriptor());
}

void Server::sendMessageToAll(const QString& message)
{
    for (ClientsIterator receiver_it = clients_.begin(); receiver_it != clients_.end(); ++receiver_it)
    {
        receiver_it->socket_->write(message.toUtf8());
        PRINT(message + " to " + receiver_it->login_)
    }
}

void Server::handleData(const QByteArray& data, int clientId)
{
    QString request = QString::fromUtf8(data).trimmed();
//    PRINT("client: " + request)

    ClientsIterator cit = clients_.find(clientId);

    if (request.startsWith("MESSAGE:"))
    {
        QString sender_login = cit->getLogin();
        QStringList message_request = request.split(":");
        QString receiver_login = message_request[1];
        QString message = message_request[2];

        PRINT("sender: " + sender_login + ", receiver:" + receiver_login)

        if (receiver_login == "all")
        {
            sendMessageToAll("MESSAGE:all:" + sender_login + ":" + message);
        }

        else if (is_logined(receiver_login))
        {
            quintptr receiver_socketDescriptor = 0;

            for (auto it = logins_.begin(); it != logins_.end(); ++it)
            {
                if (it.value() == receiver_login)
                {
                    receiver_socketDescriptor = it.key();
                    break;
                }
            }

            ClientsIterator receiver_it = clients_.find(receiver_socketDescriptor);
            QString message_answer = "MESSAGE:" + sender_login + ":" + message;
            receiver_it->socket_->write(message_answer.toUtf8());

            PRINT(message_answer)
        }

        else
        {
            // TODO: add error answer to the client
            PRINT("No such user")
        }
    }

    else if (request.startsWith("AUTH:"))
    {
        QString login = request.mid(5); // get login from the 1st line of request
        if (checkLogin(login))   // check if login valid
        {
            logins_.insert(cit->socket_->socketDescriptor(), login);
            cit->setLogin(login);

            cit->socket_->write(((QString)"AUTH:SUCCESS").toUtf8());
//            cit->socket_->flush();
            PRINT("AUTH SUCCESS!!!")
            PRINT("Send client connection status - YES")
        }
        else
        {
            PRINT("AUTH UNSUCCESS... Already have " + login + " login")

            cit->socket_->write(((QString)"AUTH:UNSUCCESS").toUtf8());
            PRINT("Send client connection status - YES")
        }
    }

    else if (request.startsWith("USERS:"))
    {
        handleUsersRequest();
    }

    else if (request.startsWith("EXIT:"))
    {
        handleExitRequest();
    }

    // TODO: add more handlers
}

//void handleMessageRequest(ClientsIterator cit)
//{

//}

void Server::handleUsersRequest()
{
    const char* answer = ("USERS:" + logins_.values().join(" ")).toUtf8();

    foreach (const Client& client, clients_)
    {
        client.socket_->write(answer); // sending to all clients list of all user logins
//        client.socket_->flush();
    }

    PRINT(answer)
}

void Server::handleExitRequest()
{
    qintptr cId = ((QTcpSocket*)sender())->socketDescriptor();    // descriptor of client to disconnect
    ClientsIterator cit = clients_.find(cId);

    QString login = cit->login_;

    clientDisconnect(cit);
    clients_.remove(cId);
    logins_.remove(cId);

    if (clients_.find(cId)==clients_.end() && logins_.find(cId)==logins_.end())
        PRINT("User " + login + " is really deleted")

    // TODO: send to users message about this client has disconnected
    sendMessageToAll("EXIT:" + login);
}

void Server::clientDisconnect(ClientsIterator& cit)
{
    socket_ = cit->socket_;

    cit->socket_->disconnectFromHost();
    cit->status_ = Client::ST_DISCONNECTED;
    cit->socket_->close();

    PRINT("User" + cit->login_ + "is disconnected")
}

void Server::on_sockConnect()
{
    PRINT("Connect")
}

void Server::on_sockDisconnect()
{
    PRINT("Disconnected socket " + QString::number(socket_->socketDescriptor()))

    if (socket_)
        socket_->deleteLater();
}

void Server::removeDisconnectedClients()   // TODO: rewrite the function
{
    ClientsIterator freeClient = clients_.end();

    for(ClientsIterator cit = clients_.begin(); cit != clients_.end(); cit++)
    {
//        if(freeClient == cit)
//            freeClient = clients_.end();

        if (cit->socket_ && !cit->socket_->isValid())  // check socket for valideness
        {
            socket_ = cit->socket_;

            logins_.remove(cit.key());     // remove login of this user from the logins_
//            clients_.remove(cit.key());    // remove the client from the clients_

//            PRINT(clients_.keys())
//          PRINT("Remove unexpected exited user " + cit->login_)

//            socket_ = cit->socket_;
        }
    }
}

void Server::on_sockError(QAbstractSocket::SocketError error)
{
    PRINT("Socket error " + QString::number(error))

    switch(error)
    {
        case QAbstractSocket::SocketError::RemoteHostClosedError:   // unexpected exit
        {
            // TODO: delete users
            removeDisconnectedClients();
            break;
        }

    default:
        {

        }
    }
}

bool Server::is_logined(QString& login) // check if login available
{
    return logins_.values().contains(login);
}

bool Server::checkLogin(QString& login) // check if login available
{
    if (!logins_.values().contains(login))
    {
        PRINT("client " + login + " connected")
        return true;
    }

    return false;
}

void Server::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    PRINT("timer tick")

//    ClientsIterator freeClient = clients_.end();

//    for(ClientsIterator cit = clients_.begin(); cit != clients_.end(); cit++)
//    {
//        if(cit->status_ == Client::ST_DISCONNECTED)
//        {
//           if(freeClient == cit)
//              freeClient = clients_.end();

//           cit = clients_.erase(cit);
//           continue;
//        }

//        if(cit->socket_->write("PING:") != -1)
//        {
//            if(!cit->socket_->waitForReadyRead(1000))
//            {
//                PRINT("Client " + cit->login_ +  " don't answer.")
//                cit = clients_.erase(cit);
//                continue;
//            }
//        }
//        else
//        {
//            PRINT("Client " + cit->login_ +  " exited urgently.")
//        }


//    }
}
