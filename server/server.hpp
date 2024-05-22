#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QTextBrowser>
#include <QLabel>
#include "client.hpp"
#include "gamecontroller.hpp"
#include "dbcontroller.hpp"
#include <QDateTime>

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
    ClientsIterator findClient(QString& login);
    void handleData(const QByteArray& data, int clientId);
    void clientDisconnect(ClientsIterator& cit);
    void handleUsersRequest();
    void handleUpdateRequest();
    void handleReadinessRequest();
    void handleConnectionRequest();
    void handleExitRequest();
    void handleFieldRequest();
    void sendMessageToAll(const QString& message);
    void removeDisconnectedClients();
    void drawKilledShip(ClientsIterator cIt, int x, int y);
    void sendFieldDrawToUsers(ClientsIterator cIt);
    void sendGamesHistoryListToUsers(QStringList& gamesHistoryList);

    void startGame(QString login1, QString login2);
    void finishGame(int gameId);

    void testDB();

private:
    quint16 port_; // QSerialPort
    QTcpSocket* socket_;
    QByteArray data_;
    Clients clients_;
    QMap<quintptr, QString> logins_;
    ServerState state_;
    int timerId_;
    Games games_;
    DBController dbController_;

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
