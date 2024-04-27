#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server(int port);
    ~Server();

    bool checkLoginRequest(QString& request);
    void handleData();

private:
    int port_; // QSerialPort
    QTcpSocket* socket;
    QByteArray data;

public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockConnect();
    void sockReady();
    void sockDisconnect();
    void sockError(QAbstractSocket::SocketError error);
};

#endif // SERVER_H
