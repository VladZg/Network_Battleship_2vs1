#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QTextBrowser>
#include "client.h"

class Server: public QTcpServer
{
    Q_OBJECT

public:
    enum ServerState
    {
        ST_NSTARTED = 0,
        ST_STARTED     ,
        ST_STOPPED     ,
    };

public:
    Server(quint16 port);
    Server(const Server& other); // copy constructor
    Server& operator=(const Server& other); // copy asignment
    ~Server();

    QTextBrowser* browser;

    void updateState(ServerState state);
    ServerState getServerState();

    bool checkLogin(QString& login);
    bool is_logined(QString& login);
    void handleData(const QByteArray& data, int clientId);
    void clientDisconnect(ClientsIterator& cit);
    void handleUsersRequest();
    void handleExitRequest();
    void sendMessageToAll(const QString& message);
    void removeDisconnectedClients();

private:
    quint16 port_; // QSerialPort
    QTcpSocket* socket_;
    QByteArray data_;
    Clients clients_;
    QMap<quintptr, QString> logins_;
    ServerState state_;
    int timerId_;

protected:
    void timerEvent(QTimerEvent* event);


public slots:
    void startServer(QTextBrowser* textBrowser);
    void stopServer();
    void incomingConnection(qintptr socketDescriptor);
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);
};

#endif // SERVER_H
