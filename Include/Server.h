#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H
