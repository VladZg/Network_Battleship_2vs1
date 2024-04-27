#ifndef CLIENT_H
#define CLIENT_H

#include <QMap>
#include <QTcpSocket>

class Client
{
public:
    Client();
    ~Client();

    typedef QMap<int, Client>::iterator ClientIterator;

    enum ClientStatus
    {
        ST_DISCONNECTED = 0,
        ST_CONNECTED       ,
        ST_AUTHORIZED      ,
        ST_READY           ,
        ST_WAITING_STEP    ,
        ST_MAKING_STEP     ,
        // more
    };

    QString getLogin();

public:
    QTcpSocket*  socket_;
    ClientStatus status_;
//    ClientIterator playingWith;
    QString login_;
};

typedef QMap<int, Client> Clients;
typedef Clients::iterator ClientsIterator;

#endif // CLIENT_H
