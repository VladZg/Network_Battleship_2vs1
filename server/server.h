#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include "client.h"

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server(quint16 port);
    ~Server();

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

protected:
    void timerEvent(QTimerEvent* event);


public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);
};

#endif // SERVER_H
