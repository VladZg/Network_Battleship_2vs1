#include "server.hpp"
#include "client.hpp"
#include "field.hpp"
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>


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

#define DB_PATH "data.db"

void Server::startServer(QTextBrowser* textBrowser)
{
    if (!this->listen(QHostAddress::Any, port_))
    {
        qDebug() << "Not listening";
        QMessageBox::warning(textBrowser, "ERROR!", "Cannot start server on port " + QString::number(port_) + "... Try again!");
        return;
    }

    dbController_.connectDatabase(DB_PATH);
    dbController_.printTable("Fields");
    dbController_.printTable("GamesEndings");

    browser = textBrowser;
//    qInstallMessageHandler([this](QtMsgType type, const QMessageLogContext& context, const QString& msg) {qDebug(msg.toUtf8()); browser->append(msg); });

    PRINT("Listening")
    updateState(ST_STARTED);

    textBrowser->setStyleSheet("background-color: rgb(255, 255, 255, 5);"\
                               "background-image: url(:/images/images/background.jpg);"\
                               "color: rgb(255, 255, 177);");

    timerId_ = startTimer(DEFAULT_SEARCH_INTERVAL);

    dbController_.createTable("Fields", "field_text TEXT");
    dbController_.createTable("GamesEndings", "player1 TEXT, player2 TEXT, field_text1 TEXT, field_text2 TEXT, start_date DATE, end_date DATE, winner TEXT");
}

void Server::stopServer()
{
    killTimer(timerId_);

    sendMessageToAll("STOP:");
    PRINT("server: STOP: to all clients")
    updateState(ST_STOPPED);

    // TODO: finish the function

    dbController_.disconnectDatabase();
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
    client.readiness_ = Client::ST_NREADY;
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

ClientsIterator Server::findClient(QString& login)
{
    for (ClientsIterator cit = clients_.begin(); cit != clients_.end(); ++cit)
    {
        if (cit->login_ == login)
            return cit;
    }

    return clients_.end();
}


void Server::on_receiveData()
{
    socket_ = (QTcpSocket*)sender();
    data_ = socket_->readAll();

    QList<QByteArray> dataArr = data_.split('@');
    for (int i = 0; i < dataArr.size()-1; i++)
    {
        data_ = dataArr[i];
        qDebug() << "DATA: " << data_;
        PRINT("client" + QString::number(socket_->socketDescriptor()) + ": " + data_)
        handleData(data_, socket_->socketDescriptor());
    }
}

void Server::sendMessageToAll(const QString& message)
{
    for (ClientsIterator receiver_it = clients_.begin(); receiver_it != clients_.end(); ++receiver_it)
    {
        receiver_it->socket_->write((message+"@").toUtf8());
//        receiver_it->socket_->flush();
        PRINT(message + " to " + receiver_it->login_)
    }
}

static QString convertFieldToBin(const QString& fieldStr)
{
    QString fieldStrBin = "";

    for (QString::const_iterator it = fieldStr.begin(); it != fieldStr.end(); ++it)
    {
        if (it->digitValue() == 0)
            fieldStrBin += QString::number((int)CELL_EMPTY);

        else if (it->digitValue() > 0 && it->digitValue() < 9)
            fieldStrBin += QString::number((int)CELL_SHIP);

        else
            qDebug() << "Wrong fieldStr!";
    }

    return fieldStrBin;
}

void Server::handleData(const QByteArray& data, int clientId)
{
    QString request = QString::fromUtf8(data).trimmed();
//    PRINT("client: " + request)

    ClientsIterator cit = clients_.find(clientId);
    QString sender_login = cit->getLogin();

    if (request.startsWith("MESSAGE:"))
    {
        QStringList message_request = request.split(":");
        QString receiver_login = message_request[1];
        QString message = message_request[2];

        PRINT("sender: " + sender_login + ", receiver:" + receiver_login)

//        if (request.startsWith("SHOT:"))
//        {
//            cit->socket_->write(((QString)"DOT").toUtf8());
//            cit->socket_->write(((QString)"DAMAGE").toUtf8());
//            cit->socket_->write(((QString)"KILL").toUtf8());

//        }

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
            receiver_it->socket_->write((message_answer+"@").toUtf8());
//            receiver_it->socket_->flush();

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

            cit->socket_->write(((QString)"AUTH:SUCCESS" + "@").toUtf8());
//            cit->socket_->flush();
            cit->updateState(Client::ST_AUTHORIZED);
//            cit->socket_->flush();
            PRINT("AUTH SUCCESS!!!")
            PRINT("Send client connection status - YES")
        }
        else
        {
            PRINT("AUTH UNSUCCESS... Already have " + login + " login")

            cit->socket_->write(((QString)"AUTH:UNSUCCESS" + "@").toUtf8());
            cit->socket_->flush();
            cit->updateState(Client::ST_CONNECTED);

            PRINT("Send client connection status - YES")
        }
    }

    else if (request.startsWith("USERS:"))
    {
        handleUsersRequest();
    }

    else if (request.startsWith("UPDATE:"))
    {
        handleUpdateRequest();
    }

    else if (request.startsWith("READINESS:"))
    {
//        handleReadinessRequest();
        qintptr cId = ((QTcpSocket*)sender())->socketDescriptor();    // descriptor of client to disconnect
        ClientsIterator cit = clients_.find(cId);

        QStringList message_request = request.split(":");
        Client::Readiness readiness = (Client::Readiness) message_request[1].toInt();

        cit->readiness_ = readiness;

        handleUsersRequest();   // TODO: delete it Later and write a function that dont delete all chats
    }

    else if (request.startsWith("CONNECTION:"))
    {
        QStringList message_request = request.split(":");
        QString receiver_login = message_request[1];

        if (is_logined(receiver_login))
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
            QString message_answer = "CONNECTION:" + sender_login;

            if (message_request.size() == 3)    // CONNECTION:<login1>:ACCEPT/REJECT request from the 2nd user
            {
                message_answer += ":" + message_request[2]; // CONNECTION:<login1>:ACCEPT/REJECT for the 1st user
            }

            else if (message_request.size() == 2)   // CONNECTION:<login2> request from the 1st user
            {
                // nothing
            }
            else
                qDebug() << "Wrong request";

            receiver_it->socket_->write((message_answer+"@").toUtf8());
//            receiver_it->socket_->flush();
            qDebug() << message_answer << " to " << receiver_login;

            PRINT(message_answer)
        }

        else
        {
            // TODO: add error answer to the client
            PRINT("No such user")
        }
    }

    else if (request.startsWith("GAME:"))
    {
        QStringList message_request = request.split(":");

        if (message_request.size() == 4)
        {
            if (message_request[1] == "START")  // GAME:START:<login_started>:<login_accepted>
            {
                QString login_started = message_request[2];
                QString login_accepted = message_request[3];

                // Init game for these 2 users
                startGame(login_started, login_accepted);
            }
            else
                PRINT("Wrong request")
        }

        else if (message_request.size() == 3)   // GAME:<gameId>:FINISH
        {
            if (message_request[2] == "FINISH")
            {
                int gameId = message_request[1].toInt(); // get gameId
                finishGame(gameId);
            }
            else
                PRINT("Wrong request")
        }

        else if (message_request.size() >= 5)
        {
            int gameId = message_request[1].toInt(); // get gameId
            QString login = message_request[2];

            GamesIterator gIt = games_.find(gameId);

            if (gIt == games_.end())
            {
                qDebug() << "No such game";
                return;
            }

            bool is_ClientStarted = (login == gIt->getClientStartedIt()->login_);

            if (message_request[3] == "FIELD")  // "GAME:<gameId>:<login>:FIELD:<fieldState>"
            {
                QString fieldStr = message_request[4];
                qDebug() << "Player " << login << " field from client: " << fieldStr;

                QString fieldBinStr = convertFieldToBin(fieldStr);
                qDebug() << "Player " << login << " field on server: " << fieldBinStr;

                if (is_ClientStarted)
                {
//                    gIt->setClientStartedFieldState(fieldStr);
//                    gIt->setClientStartedField(fieldBinStr);
                    gIt->getClientStartedIt()->initField(fieldBinStr);
                    qDebug() << "Started client field setted!";
                }
                else
                {
//                    gIt->setClientAcceptedFieldState(fieldStr);
//                    gIt->setClientAcceptedField(fieldBinStr);
                    gIt->getClientAcceptedIt()->initField(fieldBinStr);
                    qDebug() << "Accepted client field setted!";
                }

                gIt->incNPlaced();

                if (gIt->getNPlaced() == 2)
                {
                    QString message = "GAME:FIGHT";
                    gIt->getClientAcceptedIt()->socket_->write((message+"@").toUtf8());
//                    gIt->getClientAcceptedIt()->socket_->flush();
                    gIt->getClientStartedIt()->socket_->write((message+"@").toUtf8());
//                    gIt->getClientStartedIt()->socket_->flush();

                    gIt->updateState(GameController::GameState::ST_STARTED_STEP);
                    qDebug() << "GAME:FIGHT";
                }
            }
            else if (message_request[3] == "SHOT")  // "GAME:<gameId>:<login>:SHOT:<x>:<y>"
            {
                int x = message_request[4].toInt();
                int y = message_request[5].toInt();

//                ClientsIterator shooterIt = gIt->getClientAcceptedIt();
                ClientsIterator enemyIt = gIt->getClientStartedIt();

                if (is_ClientStarted)
                {
                    enemyIt = gIt->getClientAcceptedIt();
//                    shooterIt = gIt->getClientStartedIt();
                }

                Field f = enemyIt->getField();
                qDebug() << "bin  : " << f.getFieldStr();
                qDebug() << "state: " << f.getFieldStateStr();
                qDebug() << "draw : " << f.getFieldDrawStr();

                QString enemyLogin = enemyIt->login_;
                qDebug() << login + " -> " + enemyLogin +  ": SHOT (" + QString::number(x) + "," + QString::number(y) + ")";

                QString message = "SHOT:";

                bool isGameFinished = false;

                if (!enemyIt->isCellEmpty(x, y))
                {
                    gIt->incNDamaged(is_ClientStarted);

//                    qDebug() << "HERE1";
                    if(enemyIt->isKilled(x, y)) // TODO: drawKilledShip(x, y); <-- функция класса server, которая ещё и отправляет ответ игроку drawKilledShip(enemyIt, x, y);
                    {
                        message += "KILLED"; // временно
                        qDebug() << "Убит!";

                        drawKilledShip(enemyIt, x, y);
                        sendFieldDrawToUsers(enemyIt);

                         isGameFinished = gIt->checkGameFinish(is_ClientStarted);

//                        return;
                    }
                    else
                    {
                    //    qDebug() << "HERE2";
                        // else    // DAMAGED
                        enemyIt->setCellDraw(x, y, Field::CellDraw::CELL_DAMAGED);
                        qDebug() << "Попадание!";
                        message += "DAMAGED";
                    }
                }
                else    // DOT
                {
//                    qDebug() << "HERE3";
                    enemyIt->setCellDraw(x, y, Field::CellDraw::CELL_DOT);
                    qDebug() << "Промах!";
                    message += "DOT";

                    if (is_ClientStarted)
                    {
                        gIt->updateState(GameController::GameState::ST_ACCEPTED_STEP);
                    }
                    else
                    {
                        gIt->updateState(GameController::GameState::ST_STARTED_STEP);
                    }
                }

                message += ":" + QString::number(x) + ":" + QString::number(y);

                gIt->getClientStartedIt()->socket_->write(((QString)(message+"@")).toUtf8());
//                gIt->getClientStartedIt()->socket_->flush();
                gIt->getClientAcceptedIt()->socket_->write(((QString)(message+"@")).toUtf8());
//                gIt->getClientAcceptedIt()->socket_->flush();

                if (isGameFinished)
                {
                    gIt->updateState(GameController::GameState::ST_FINISHED);
                    gIt->winnerLogin_ = enemyIt->enemy_->getLogin();
                    qDebug() << "all ships killed! game finished!";
                    finishGame(gameId);
                    // end timer and push to database
                }
            }
            else
            {
                qDebug() << "Wrong GAME: request";
            }
        }
        else if (message_request.size() == 6)
        {


        }

        else
            PRINT("Wrong request")
    }

    else if (request.startsWith("HISTORY:UPDATE:"))
    {
        QStringList gamesHistoryList = dbController_.getGamesEndings();
        sendGamesHistoryListToUsers(gamesHistoryList);
    }

    else if (request.startsWith("GENERATE:"))  // "GENERATE:"
    {
        QString randomFieldStr = dbController_.getRandomField();

        if (randomFieldStr.size() < 100)
        {
            qDebug() << "Wrong generated by db field";
            return;
        }

        Field field = Field(randomFieldStr);
//        field.generate();
        qDebug() << "Generated field_ : " << field.getFieldStr();
        qDebug() << "Generated fieldState_: " << field.getFieldStateStr();
        qDebug() << "Generated fieldDraw_: "<< field.getFieldDrawStr();
        QString message = "GENERATE:" + field.getFieldStr();

        qintptr cId = ((QTcpSocket*)sender())->socketDescriptor();    // descriptor of client to disconnect
        ClientsIterator cit = clients_.find(cId);

        if (cit == clients_.end())
        {
            qDebug() << "No such client";
            return;
        }

        cit->socket_->write((message+"@").toUtf8());
//        cit->socket_->flush();
        qDebug() << "Client's" + cit->login_ + "field generated and sended!";
    }

    else if (request.startsWith("EXIT:"))
    {
        handleExitRequest();
    }

    else
    {

    }

    // TODO: add more handlers
}

void Server::sendFieldDrawToUsers(ClientsIterator cIt)
{
    cIt->socket_->write(("FIELD:UPDATE:MY:" + cIt->getField().getFieldDrawStr() + "@").toUtf8());
    cIt->enemy_->socket_->write(("FIELD:UPDATE:ENEMY:" + cIt->getField().getFieldDrawStr() + "@").toUtf8());
}

void printField(const QVector<Field::CellDraw>& field)
{
    int width = sqrt(field.size());
    int height = width;

    QDebug debugOut = qDebug();
    debugOut << "\n";
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           debugOut << (int)(field[i*width+j]) << " ";
        }
        debugOut << "\n";
    }
}

void Server::drawKilledShip(ClientsIterator cIt, int x, int y)
{
    qDebug() << "Drawing killed ship...!";

    // Получаем поле клиента, у которого нужно сделать отрисовку убитого корабля
    Field field = cIt->getField();
    QVector<Field::CellState> fieldState = field.getFieldState();
    QVector<Field::CellDraw> fieldDraw   = field.getFieldDraw();
    int width_  = field.getWidth();
    int height_ = field.getHeight();

    // По традиции создаём поля State и Draw размером 12x12. Делаем их пустыми
    QVector<Field::CellState> fieldStateWithBorders((width_+2)*(height_+2));
    QVector<Field::CellDraw> fieldDrawWithBorders((width_+2)*(height_+2));
    fieldStateWithBorders.fill(Field::CL_ST_EMPTY);
    fieldDrawWithBorders.fill(Field::CELL_EMPTY);

    // Копируем в них поля State и Draw размерами 10x10
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldStateWithBorders[(width_+2)*(i+1)+(j+1)] = fieldState[width_*i+j];
        }
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldDrawWithBorders[(width_+2)*(i+1)+(j+1)] = fieldDraw[width_*i+j];
        }
    }

    // Переводим координаты клетки, в которую попали, из пространства 10x10 в пространство 12x12
    int damagedCell = (width_+2)*(y+1)+(x+1);

    // Сам алгоритм отрисовки клеток убитого корабля
    switch(fieldStateWithBorders[damagedCell])
    {
        case(Field::CellState::CL_ST_CENTER):
        {
            for(int cell = damagedCell - (width_ + 2) - 1, cnt = 0; cnt < 3; ++cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 1 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 2 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
            }

            fieldDrawWithBorders[damagedCell] = Field::CellDraw::CELL_KILLED;
            break;
        }

        case(Field::CellState::CL_ST_TOP):
        {
            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell + lenght * (width_ + 2)] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell - (width_ + 2) - 1, cnt = 0; cnt < 3; ++cell, ++cnt)
            {
                for(int tmpCell = cell, tmpCnt = 0; tmpCnt < lenght + 2; tmpCell += width_ + 2, tmpCnt++)
                {
                    fieldDrawWithBorders[tmpCell] = Field::CellDraw::CELL_DOT;
                }
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; cell += width_ + 2, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        case(Field::CellState::CL_ST_LEFT):
        {
            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell + lenght] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell - (width_ + 2) - 1, cnt = 0; cnt < lenght + 2; ++cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 1 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 2 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; ++cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        case(Field::CellState::CL_ST_BOTTOM):
        {
            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell - lenght * (width_ + 2)] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell + (width_ + 2) - 1, cnt = 0; cnt < 3; ++cell, ++cnt)
            {
                for(int tmpCell = cell, tmpCnt = 0; tmpCnt < lenght + 2; tmpCell -= width_ + 2, tmpCnt++)
                {
                    fieldDrawWithBorders[tmpCell] = Field::CellDraw::CELL_DOT;
                }
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; cell -= width_ + 2, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        case(Field::CellState::CL_ST_VMIDDLE):
        {
            for(; fieldStateWithBorders[damagedCell] != Field::CellState::CL_ST_EMPTY; damagedCell -= (width_+2)) {}
            damagedCell += (width_+2);

            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell + lenght * (width_ + 2)] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell - (width_ + 2) - 1, cnt = 0; cnt < 3; ++cell, ++cnt)
            {
                for(int tmpCell = cell, tmpCnt = 0; tmpCnt < lenght + 2; tmpCell += width_ + 2, tmpCnt++)
                {
                    fieldDrawWithBorders[tmpCell] = Field::CellDraw::CELL_DOT;
                }
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; cell += width_ + 2, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        case(Field::CellState::CL_ST_HMIDDLE):
        {
            for(; fieldStateWithBorders[damagedCell] != Field::CellState::CL_ST_EMPTY; damagedCell--) {}
            damagedCell++;

            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell + lenght] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell - (width_ + 2) - 1, cnt = 0; cnt < lenght + 2; ++cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 1 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 2 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; ++cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        case(Field::CellState::CL_ST_RIGHT):
        {
            int lenght = 0;
            for(; fieldStateWithBorders[damagedCell - lenght] != Field::CellState::CL_ST_EMPTY; lenght++) {}

            for(int cell = damagedCell - (width_ + 2) + 1, cnt = 0; cnt < lenght + 2; --cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 1 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
                fieldDrawWithBorders[cell + 2 * (width_ + 2)] = Field::CellDraw::CELL_DOT;
            }

            for(int cell = damagedCell, cnt = 0; cnt < lenght; --cell, ++cnt)
            {
                fieldDrawWithBorders[cell] = Field::CellDraw::CELL_KILLED;
            }
            break;
        }

        default:
        {
            qDebug() << "From function drawKilledShip: unknown cell state!";
            break;
        }
    }

    // Переводим отрисованное поле из 12x12 в 10x10
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldDraw[width_*i+j]  = fieldDrawWithBorders[(width_+2)*(i+1)+(j+1)];
        }
    }

    cIt->setFieldDraw(fieldDraw);

//    printField(fieldDraw);
}



//void handleMessageRequest(ClientsIterator cit)
//{

//}


void Server::handleUsersRequest()
{
    QList<QString> users_list;

    foreach (const Client& client, clients_)
    {
        if (client.isAuthorized())
        {
            QString user_str = client.login_ + ":" + QString::number(client.status_) + ":" + QString::number(client.readiness_); // <login>:<status>:<readiness>
            users_list.append(user_str);
        }
    }

    QString answer = "USERS:" + users_list.join(" ");    // USERS:<login1>:<status1>:<readiness1> <login2>:<status2>:<readiness2> ...

    foreach (const Client& client, clients_)
    {
        if (client.isAuthorized())
        {
            client.socket_->write(((answer+"@").toUtf8())); // sending to all clients list of all user logins
//            client.socket_->write(((QString)"\n").toUtf8()); // sending to all clients list of all user logins
            client.socket_->flush();

            PRINT("to " + client.login_ + " : " + answer)
        }
    }
}

void Server::handleUpdateRequest()
{
    QList<QString> users_list;

    foreach (const Client& client, clients_)
    {
        if (client.isAuthorized())
        {
            QString user_str = client.login_ + ":" + QString::number(client.status_) + ":" + QString::number(client.readiness_); // <login>:<status>:<readiness>
            users_list.append(user_str);
        }
    }

    const char* answer = ("USERS:" + users_list.join(" ")).toUtf8();    // USERS:<login1>:<status1>:<readiness1> <login2>:<status2>:<readiness2> ...


    qintptr cId = ((QTcpSocket*)sender())->socketDescriptor();    // descriptor of client to disconnect
    ClientsIterator cit = clients_.find(cId);
    cit->socket_->write(((QString)answer+"@").toUtf8()); // sending to all clients list of all user logins
    cit->socket_->flush();

    PRINT(answer)
}

void Server::handleReadinessRequest()
{

}

void Server::handleConnectionRequest()
{
//    QStringList message_request = request.split(":");

}

void Server::handleExitRequest()
{
    qintptr cId = ((QTcpSocket*)sender())->socketDescriptor();    // descriptor of client to disconnect
    ClientsIterator cit = clients_.find(cId);

    qDebug() << "sender: " << cit->login_;

    QString login = cit->login_;

    clientDisconnect(cit);
    clients_.erase(cit);
    logins_.remove(cId);
    qDebug() << login + " removed from clients_ and logins_";

    if (clients_.find(cId)==clients_.end() && logins_.find(cId)==logins_.end())
        PRINT("User " + login + " is really deleted")

    handleUsersRequest();

    // TODO: send to users message about this client has disconnected
//    sendMessageToAll(":" + login);
}

void Server::handleFieldRequest()
{

}

void Server::clientDisconnect(ClientsIterator& cit)
{
//    socket_ = cit->socket_;

    cit->socket_->disconnectFromHost();
    cit->status_ = Client::ST_DISCONNECTED;
    cit->socket_->close();

    PRINT("User " + cit->login_ + " is disconnected")
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
//    PRINT("timer tick")

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

void Server::startGame(QString login_started, QString login_accepted)
{
    ClientsIterator c1It = findClient(login_started);
    ClientsIterator c2It = findClient(login_accepted);

    c1It->enemy_ = c2It;
    c2It->enemy_ = c1It;

    static int gameId = 0;
    gameId++;   // get gameId

    GameController gameController(gameId, c1It, c2It);

    // start timer
    gameController.startTime_  = QDateTime::currentDateTime();
    gameController.startDate_ = QDate::currentDate();
//    qDebug() << "Время начала:" << gameController.startTime_.toString("hh:mm:ss");

    games_.insert(gameId, gameController);

    if (games_.find(gameId) != games_.end())
        PRINT("New game inserted in games_")
    else
        PRINT("Error in inserting new game")

    QString message1 = "GAME:START:" + login_accepted + ":" + QString::number(gameId);
    QString message2 = "GAME:START:" + login_started + ":" + QString::number(gameId);

//    c1It->readiness_ = Client::ST_PLAYING;
//    c2It->readiness_ = Client::ST_PLAYING;

    c1It->socket_->write((message1+"@").toUtf8());
//    c1It->socket_->flush();
    c2It->socket_->write((message2+"@").toUtf8());
//    c2It->socket_->flush();

    gameController.updateState(GameController::GameState::ST_PLACING);

    qDebug() << message1;
    qDebug() << message2;
    PRINT("Start game " + login_started + " vs " + login_accepted + " with gameId=" + QString::number(gameId))
}

void Server::finishGame(int gameId)
{
    GamesIterator gameIt = games_.find(gameId);

    qDebug() << "We have now " + QString::number(games_.size()) + " active games";

    for (GamesIterator git = games_.begin(); git != games_.end(); ++git)
    {
        PRINT("game " + QString::number(git->getGameId()) + " " + git->getClientStartedIt()->login_ + " vs " + git->getClientAcceptedIt()->login_)
    }

    if (gameIt == games_.end())
    {
        PRINT("Game " + QString::number(gameId) + " not found")
        return;
    }

    ClientsIterator c1It = gameIt->getClientStartedIt();    // get from game structure
    ClientsIterator c2It = gameIt->getClientAcceptedIt();   // get from game structure

    QString login1 = c1It->login_;
    QString login2 = c2It->login_;

    PRINT("login1: " + login1 + ", login2: " + login2)

    QString message = "GAME:";

    // проверка на то, что игра закончилась, и если игра прервана пользователем, то отправляем GAME:STOP:<>...
    if (gameIt->getState() == GameController::ST_FINISHED)
    {
        if (gameIt->winnerLogin_ == "")
        {
            qDebug() << "Game " << gameId << " has no winner yet...";
            return;
        }

        message += "FINISH:" + gameIt->winnerLogin_;
        PRINT("Finish game " + login1 + " vs " + login2 + "with gameId=" + QString::number(gameId))

        // Заполняем базу данных завершившейся игрой
        gameIt->endTime_ = QDateTime::currentDateTime();
        gameIt->endDate_ = QDate::currentDate();
        dbController_.addNewGameEnding(gameIt);
        dbController_.printTable("GamesEndings");
        QStringList gamesHistoryList = dbController_.getGamesEndings();
        sendGamesHistoryListToUsers(gamesHistoryList);
    }
    else
    {
        message += "STOP";
        PRINT("Stop game " + login1 + " vs " + login2 + "with gameId=" + QString::number(gameId))
    }

//    c1It->readiness_ = Client::ST_NREADY;
//    c2It->readiness_ = Client::ST_NREADY;

    c1It->socket_->write((message+"@").toUtf8());
//    c1It->socket_->flush();
    c2It->socket_->write((message+"@").toUtf8());
//    c2It->socket_->flush();

    PRINT(message + " to " + login1)
    PRINT(message + " to " + login2)
    PRINT("Done")

//    gameIt->~GameController();  // finish game
    games_.erase(gameIt);
}

void Server::testDB()
{
    const QString &fileName = ":/placements.txt";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open file" << fileName;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        qDebug() << line << "@";
        dbController_.addNewPlacement(line);
    }

    file.close();

    dbController_.printTable("Fields");

//    QString randomFieldStr = dbController_.getRandomField();
//    qDebug() << "Random field: " + randomFieldStr;

//    dbController_.clearDatabase();
//    dbController_.printTable("Fields");
//    dbController_.printTable("GameEndings");
}

void Server::sendGamesHistoryListToUsers(QStringList& gamesHistoryList)
{
    QString message = "HISTORY:UPDATE:" + gamesHistoryList.join("$$");

    qDebug() << "GameEndings table: " << message;
    sendMessageToAll(message);
}
