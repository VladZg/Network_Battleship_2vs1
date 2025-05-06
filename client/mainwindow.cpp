#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "images.hpp"
#include "fightshistorywindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextBrowser>
#include <QTextCharFormat>
#include <QPainter>
#include <QPaintEngine>
#include <iostream>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QTableWidget>

#define CLICK_SOUND controller_->playSound("click");

MainWindow::MainWindow(QString ip, quint16 port, QWidget *parent)
    : QMainWindow(parent)
    , ip_(ip)
    , port_(port)
    , ui(new Ui::MainWindow)
    , fightsHistoryWindow_(FightsHistoryWindow(this))
{
    ui->setupUi(this);

    receiverBrowserStackedWidget = new QStackedWidget(this);

    QHBoxLayout* chatWidgetLayout = new QHBoxLayout(this);
    QVBoxLayout* receiverListWidgetLayout = new QVBoxLayout(this);

    chatWidgetLayout->addWidget(receiverBrowserStackedWidget);
    receiverListWidgetLayout->addWidget(ui->messageRecieversOptionList);

    chatWidgetLayout->setGeometry(ui->chatWidget->geometry());
    ui->chatWidget->setLayout(chatWidgetLayout);
    ui->messageRecieversOptionsListWidget->setLayout(receiverListWidgetLayout);

    ui->updateButton->setVisible(false);
    ui->messageEdit->setVisible(false);
    ui->chatWidget->setVisible(false);
    ui->messageRecieversOptionList->setVisible(false);
    ui->updateButton->setVisible(false);
    ui->sendMessageButton->setVisible(false);
    ui->menubar->setVisible(false);

    ui->gameExitButton->setVisible(false);
    ui->applyFieldButton->setVisible(false);
    ui->generateFieldButton->setVisible(false);
    ui->fieldsLabel->setVisible(false);
    ui->checkButton->setVisible(false);
    ui->clearButton->setVisible(false);
    ui->applyIsOkLabel->setVisible(false);
    ui->applyIsNotOkLabel->setVisible(false);
//    ui->isReadyCheckBox->setVisible(false);

    ui->isReadyCheckBox->setStyleSheet("QCheckBox { color: black; }"
                                   "QCheckBox::indicator { background-color: red; }");
    ui->isReadyCheckBox->setText("не авторизован");
    ui->isReadyCheckBox->setEnabled(false);

    connect(ui->messageRecieversOptionList, SIGNAL(itemSelectionChanged()), this, SLOT(on_messageRecieversOptionList_itemSelectionChanged()));

    socket_ = new QTcpSocket(this);

    userLogins_ = QStringList(); // list of user logins

    pictures.load();        // loading all the images for the game
    model_ = new Model();   // game model with fields
    controller_ = new Controller(model_, socket_);

    QGraphicsScene* graphicsScene = new QGraphicsScene;
    QGraphicsView* graphicsView = new QGraphicsView;
    graphicsView->setScene(graphicsScene);
    graphicsView->setParent(ui->fieldsLabel);

    connect(ui->openFightHistoryAction, SIGNAL(triggered), this, SLOT(on_openFightHistoryAction_triggered()));

    connectionStateUpdate(ST_DISCONNECTED);

    connect(socket_, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(socket_, SIGNAL(connected())   , this, SLOT(on_sockConnect())   );  // when socket connected, sockConnect slot runs
    connect(socket_, SIGNAL(readyRead())   , this, SLOT(on_receiveData())     );  // when new network data comes, sockReady slot runs
    connect(socket_, SIGNAL(disconnected()), this, SLOT(on_sockDisconnect()));  // when socket disconnected, sockDisconnect slot runs

    timerId_ = startTimer(2000);
//    connectUser();
}

MainWindow::~MainWindow()
{
    connectionStateUpdate(ST_DISCONNECTED);

    delete model_;
    delete controller_;
    delete ui;
}

void MainWindow::connectionStateUpdate(ClientConnectionState new_state)
{
    switch (new_state)
    {
        case ST_DISCONNECTED:
        {
            ui->statusbar->showMessage("status: ST_DISCONNECTED");
            break;
        }
        case ST_CONNECTED:
        {
            ui->statusbar->showMessage("status: ST_CONNECTED");
            break;
        }
        case ST_AUTHORIZED:
        {
            ui->statusbar->showMessage("status: ST_AUTHORIZED");
            break;
        }
    }

    connectionState_ = new_state;
}

void MainWindow::on_connectToServerButton_clicked()    // connection+authorization button
{
    CLICK_SOUND

    if (connectionState_ == ST_DISCONNECTED)
    {
        qDebug() << "Connect button pushed";
        QMessageBox::warning(this, "ERROR!", "Cannot connect user to server... Server not started yet");
        return;
    }

    if (connectionState_ == ST_CONNECTED)
    {
        authenticateUser();
    }

//    if (connectionState_ == ST_AUTHORIZED)
//        return;
}

void MainWindow::connectUser()
{
    socket_->connectToHost(ip_, port_);

    if (!socket_->waitForConnected(1000))
    {
        qDebug() << "Cannot connect";
        qDebug() << socket_->error();
        return;
    }

    connectionStateUpdate(ST_CONNECTED);
    ui->connectToServerButton->setText("Авторизоваться на сервере");
    qDebug() << "Authorized";

    killTimer(timerId_);
}

void MainWindow::authenticateUser()
{
    QString login_entered = ui->loginLabel->text();

    if (!login_entered.isEmpty())
    {
        qDebug() << "trying to connect to server with login " << login_entered << "";

        if (login_entered.split(" ").size() > 1)
        {
            QMessageBox::warning(this, "Login error!", "Неверный формат логина! Логин не должен содержать пробелов и специальных символов. Попробуйте снова");

            ui->loginLabel->clear();

            qDebug() << "Wrong login format! Must not have spaces";
            return;
        }

        socket_->write(("AUTH:" + login_entered + "@").toUtf8()); // request for authorization
//        socket_->flush();

        if (!socket_->waitForReadyRead(2500)) // if server don't answer > 2.5 sec
        {
            qDebug() << "Server don't answer";
            return;
        }

        QString answer = QString::fromUtf8(data_).trimmed();    // gettin g server answer
        if (answer.startsWith("AUTH:SUCCESS"))
        {
            connectionStateUpdate(ST_AUTHORIZED);
            login_ = login_entered;

            controller_->playSound("intro_music");

            ui->loginLabel->setReadOnly(true); // block loginLabel for writing after user if authorized

//            QString login = login_entered.trimmed();
//            ui->usersList->addAction(login);

            ui->loginLabel->setStyleSheet("background-color: rgb(143, 240, 164);");

            qDebug() << "loggining confirmed!";

            ui->connectToServerButton->setText("Вы успешно авторизованы!");
            ui->connectToServerButton->setEnabled(false);   // turn the button off

            ui->updateButton->setVisible(true);
            ui->messageEdit->setVisible(true);
            ui->chatWidget->setVisible(true);
            ui->messageRecieversOptionList->setVisible(true);
            ui->updateButton->setVisible(true);
            ui->sendMessageButton->setVisible(true);
            ui->menubar->setVisible(true);

//            ui->authLayout->
//            ui->authLayout->(500, 500);
//            QPoint newPos = ui->loginLabel->geometry().topLeft() + QPoint(300, 300);
//            ui->loginLabel->setGeometry(QRect(newPos, ui->authLayout->geometry().size()));
//            ui->loginLabel->setCursorPosition();
//            ui->authLayout->setParent(ui->centralwidget);
//            ui->authLayout->setGeometry(QRect(0,0,100,200)); //move(500,500);

            ui->clearButton->setVisible(true);
            ui->generateFieldButton->setVisible(true);
            ui->fieldsLabel->setVisible(true);
            ui->checkButton->setVisible(true);
            ui->applyIsNotOkLabel->setVisible(true);
            ui->applyIsOkLabel->setVisible(false);

            model_->setLogin(login_);

            ui->messageRecieversOptionList->addItem("all");

            // TODO: get list of users from server
//            makeUsersRequest();

            updateReadiness(ST_NREADY);
            ui->isReadyCheckBox->setVisible(true);
            ui->isReadyCheckBox->setEnabled(true);
        }

         else // didn't authorized
        {
            ui->loginLabel->setStyleSheet("background-color: red;\n color: white;");
            QMessageBox::warning(this, "ERROR!", "Login is already in use");
        }
    }
}

void MainWindow::on_sockConnect()
{

}

void MainWindow::on_sockDisconnect()
{
    socket_->deleteLater();     // delete socket
}

void MainWindow::on_sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}

void MainWindow::on_openFightHistoryAction_triggered()
{
    qDebug() << "show fight history window";
    socket_->write(((QString)"HISTORY:UPDATE:@").toUtf8());
    qDebug() << "to server: HISTORY:UPDATE:";
//    socket_->waitForReadyRead(2000);

    fightsHistoryWindow_.update();

    fightsHistoryWindow_.setEnabled(true);
    fightsHistoryWindow_.show();
    fightsHistoryWindow_.activateWindow();
    fightsHistoryWindow_.raise();
    fightsHistoryWindow_.exec();
}

void MainWindow::on_userToChose_triggered()
{
    qDebug() << "Send request to ..." ; // TODO
}

void MainWindow::on_receiveData()
{
//    socket->waitForReadyRead(500);
    data_ = socket_->readAll();
    qDebug() << "server: " << data_;

    QList<QByteArray> dataArr = data_.split('@');
    for (int i = 0; i < dataArr.size()-1; i++)
    {
        data_ = dataArr[i];
//        PRINT("client" + QString::number(socket_->socketDescriptor()) + ": " + data_)
        handleData();
    }

    this->update();
}

void MainWindow::handleData()
{
    if (data_.startsWith("CONNECTION:"))
    {
        handleConnectionRequest();
    }

    if (connectionState_ != ST_AUTHORIZED)
        return;

    if (data_.startsWith("MESSAGE:"))
    {
        handleMessageRequest();
    }

    else if(data_.startsWith("USERS:"))
    {
        handleUsersRequest();
    }

    else if (data_.startsWith("FIELD:"))
    {
        handleFieldRequest();
    }

    else if(data_.startsWith("SHOT:"))
    {
        handleShotRequest();
    }

    else if(data_.startsWith("PING:"))
    {
        handlePingRequest();
    }

    else if (data_.startsWith("GAME:"))
    {
        handleGameRequest();
    }

    else if (data_.startsWith("GENERATE:"))
    {
        handleGenerateRequest();
    }

    else if(data_.startsWith("EXIT:"))
    {
        handleExitRequest();
    }

    else if(data_.startsWith("STOP:"))
    {

        stopClient("Server stopped... Closing the app");
    }

    else if (data_.startsWith("HISTORY:UPDATE:"))
    {
        handleHistoryUpdateRequest();
    }
    else
    {

    }
}

// TODO: create a method that updates (creates/removes) all chat QTextBrowser's


void MainWindow::handlePingRequest()
{
    socket_->write(((QString)"PONG:" + "@").toUtf8());
//    socket_->flush();
}

void MainWindow::handleMessageRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");

    if (message_request.length() < 2)
    {
        qDebug() << "ERROR";
        return;
    }

    QString chat_with = message_request[1];
    QList<QListWidgetItem*> sendersList = ui->messageRecieversOptionList->findItems(chat_with, Qt::MatchExactly);

    if (sendersList.isEmpty())
    {
        qDebug() << "No such chat in messageRecieversOptionList";
        return;
    }

    QListWidgetItem* sender = sendersList.first();  // specific sender from the messageRecieversOptionList
//        qDebug() << sender;
    QTextBrowser* chat = browserMap.value(sender);    // look for specific chat

    if (!chat)
    {
//            qDebug() << "Error: no such chat in browserMap";
//            return;

        createTextBrowser(sender);
        chat = browserMap.value(sender);
    }

    int shift = 0;

    if (message_request[1]=="all")
    {
        shift = 1;
    }

    QString sender_login = message_request[1+shift];
    QString message = message_request[2+shift];

    if (sender_login == login_) // if message from myself
        return;

    QTextCharFormat receiver_format;
    QTextCharFormat message_format;

    receiver_format.setForeground(QColor("green"));
    message_format.setForeground(QColor("black"));

    chat->textCursor().insertText(sender_login + "> ", receiver_format); // show received message in corresponding chat
    chat->textCursor().insertText(message + "\n", message_format);

//        chat->append(sender_login + "> " + message);    // show received message in common chat (all)

    if (ui->messageRecieversOptionList->currentItem() != sender)   // if message to not a current chat then we set a flag to unchecked (then change the color)
    {
        controller_->playSound("new_msg");
        sender->setBackground(QBrush(QColor("green")));
        sender->setForeground(QBrush(QColor("white")));
        sender->setData(Qt::UserRole, true); // new flag means about new message not read yet
    }
}

void MainWindow::handleShotRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");

    if (message_request.size() == 4)
    {
        int x = message_request[2].toInt();
        int y = message_request[3].toInt();

        QString shotResult = message_request[1];
        CellDraw status = CELL_EMPTY;

        if (shotResult == "DOT")
        {
            status = CELL_DOT;
        }
        else if (shotResult == "DAMAGED")
        {
            status = CELL_DAMAGED;
        }
        else if (shotResult == "KILLED")
        {
            status = CELL_KILLED;
//            controller_->playSound("you_hit");
            qDebug() << "Вы подорвали корабль противника! Продолжайте ход";
        }
        else
        {

        }

        if (model_->getState() == ST_MAKING_STEP)
        {
            model_->setEnemyCell(x, y, status);
            qDebug() << "Enemy field: (" + QString::number(x) + "," + QString::number(y) + ") = " + QString::number(status) + "(" + shotResult + ")";

            if (status == CELL_DOT)
            {
                controller_->playSound("enemy_miss");
                qDebug() << "Вы промазали, смена хода!";
                model_->switchStep();
                ui->whooseStepLabel->setText("Ход соперника");
            }
            else if (status == CELL_DAMAGED)
            {
                controller_->playSound("enemy_hit");
                qDebug() << "Вы попали! Продолжайте ход";
            }
            else if (status == CELL_KILLED)
            {
                controller_->playSound("enemy_kill");
                qDebug() << "Вы подорвали корабль противника! Продолжайте ход";
            }
            else
            {

            }
        }
        else if (model_->getState() == ST_WAITING_STEP)
        {
            model_->setMyDrawCell(x, y, status);
            qDebug() << "My field: (" + QString::number(x) + "," + QString::number(y) + ") = " + QString::number(status) + "(" + shotResult + ")";

            if (status == CELL_DOT)
            {
                controller_->playSound("you_miss");
                qDebug() << "Противник промазал, смена хода!";
                model_->switchStep();
                ui->whooseStepLabel->setText("Ваш ход");
//                return;
            }
            else if (status == CELL_DAMAGED)
            {
                controller_->playSound("you_hit");
                qDebug() << "Противник попал и продолжает ход";
            }
            else if (status == CELL_KILLED)
            {
                controller_->playSound("you_kill");
                qDebug() << "Противник попал и уничтожил ваш корабль! Его ход";
            }
        }

        update();
    }
    else
    {
        qDebug() << "Wrong request";
        return;
    }
}

void MainWindow::makeUsersRequest() // requests list of users and add connected/remove disconnected them in usersList and in messageRecieversOptionList
{
    socket_->write(((QString)"USERS:" + "@").toUtf8());
//    socket_->flush();
//    socket_->waitForReadyRead(500);

//    handleUsersRequest();
}

void MainWindow::updateUsers(QStringList users_list)
{
//    ui->messageRecieversOptionList->addItem("all");

//    ui->usersList->clear();
    users_.clear();
    users_.append("all");

    // update users to users_list
    for (int i = 0; i < users_list.size(); ++i)
    {
      QStringList parts = users_list[i].split(":");
      if (parts.size() > 0)
      {
        users_.append(parts[0]);
      }
    }

    QHash<QString, bool> usersHash;
    for (int i = 0; i < ui->messageRecieversOptionList->count(); ++i)
    {
      usersHash.insert(ui->messageRecieversOptionList->item(i)->text(), true);
    }

    // Перебрать users и обновить ui->usersList
    for (int i = 0; i < users_.size(); ++i)
    {
      if (usersHash.contains(users_[i]))
      {
        usersHash.remove(users_[i]);
      }
      else
      {
        ui->usersList->addAction(users_[i]);
        ui->messageRecieversOptionList->addItem(users_[i]);
      }
    }

    // Удалить оставшиеся элементы из ui->usersList
    for (auto it = usersHash.begin(); it != usersHash.end(); ++it)
    {
        qDebug() << "void updateUsers(): <---here1";

      for (int i = 1; i < ui->messageRecieversOptionList->count(); ++i)
      {
          qDebug() << "void updateUsers(): <---here2";

        if (ui->messageRecieversOptionList->item(i)->text() == it.key())
        {
          qDebug() << "user i to remove: " << i-1 << " " << ui->usersList->actions()[i-1];
          ui->usersList->actions().removeAt(i-1);
          QAction* userAction = ui->usersList->actions().at(i-1);
          delete userAction;

          qDebug() << "void updateUsers(): <---here3";

          QListWidgetItem* userItem = ui->messageRecieversOptionList->takeItem(i);
          delete userItem;

          qDebug() << "void updateUsers(): <---here4";

          break;
        }
      }
    }

    qDebug() << "users_: " << users_;
    qDebug() << "ui->usersList: " << ui->usersList->actions();

    ui->menubar->update();
    ui->messageRecieversOptionList->update();

    qDebug() << "updated userList: " << ui->usersList;
    qDebug() << "updated messageRecieversOptionList: " << ui->messageRecieversOptionList;
}

void MainWindow::updateChats()
{
    for (int i = 0; i < ui->messageRecieversOptionList->count(); ++i)
    {
        QListWidgetItem* receiver = ui->messageRecieversOptionList->item(i);
        QTextBrowser* chat = browserMap.value(receiver);

        if (!chat)
        {
            createTextBrowser(receiver);
        }
    }

    // TODO: add remove chats function
}

void MainWindow::setIconStatus(QAction* userToChoose, int readiness)
{
    if (readiness == 0)    // ST_NREADY
    {
        userToChoose->setIcon(QIcon(":/images/st_nready.jpg"));
    }
    else if (readiness == 1)   // ST_READY
    {
        userToChoose->setIcon(QIcon(":/images/st_ready.png"));
    }
    else if (readiness == 2)   // ST_PLAYING
    {
        userToChoose->setIcon(QIcon(":/images/st_playing.jpeg"));
    }
    else
    {
        qDebug() << "unknown status";
    }
}

void MainWindow::handleUsersRequest()
{
//    qDebug() << "HANDLE";

    QStringList users_list = QString::fromUtf8(data_.trimmed().mid(6)).split(" ", Qt::SkipEmptyParts); // getting list of users (login:status:readiness) from the request
//    qDebug() << "user_list: " << users_list;
//    qDebug() << "ui->usersList: " << ui->usersList->actions();
//    qDebug() << "ui->usersList:" << ui->usersList->actions();

    QString cur_login = "";

    QListWidgetItem* cur_user = ui->messageRecieversOptionList->currentItem();
    if (cur_user)
        cur_login = cur_user->text();

    updateUsers(users_list);
    updateChats();
    ui->usersList->clear();

    int cur_row_index = -1; // when we have "all" it will be = 0
    int new_cur_row_index = cur_row_index;
//    int i = 0;

//    QList<QAction*> usersListActions = ui->usersList->actions();

    foreach (QString user, users_list) // QAction* userIt, ui->usersList->actions())
    {
        qDebug() << "user = " << user;

        QStringList user_info = user.split(":");
        QString login = user_info[0];
        int status = user_info[1].toInt();
        int readiness = user_info[2].toInt();

        QAction* userIt = ui->usersList->addAction(login);
        setIconStatus(userIt, readiness);

        if (status == 2 && readiness == 1 && login != login_) // == ST_AUTHORIZED && == ST_READY
        {
            qDebug() << "connecting button to start game handler";

            QAction* userToPlayWith = userIt; // ui->readyPlayersList->addAction(userToChoose->text());

            userToPlayWith->setEnabled(true);
            QObject::connect(userToPlayWith, &QAction::triggered, [this, userToPlayWith]()
            {
                QString enemy_login = userToPlayWith->text();

                CLICK_SOUND
                qDebug() << "try to connect to the user " << enemy_login;
                // TODO: handler

                connectToGame(enemy_login);
            });
        }

        else if (login == login_)
        {
            userIt->setEnabled(false);
        }

        else
        {

        }

        cur_row_index++;

        qDebug() << "User " << login;

        if (cur_login == login)
            new_cur_row_index = cur_row_index;
    }

//    ui->messageRecieversOptionList->setCurrentRow(new_cur_row_index);   // set message to all at default
}

void MainWindow::handleFieldRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");

    if (message_request.size() < 4 || message_request[1] != "UPDATE")
    {
        qDebug() << "wrong request";
        return;
    }

    QString fieldDrawStr = message_request[3];
//    qDebug() << "fieldDrawStr = " << fieldDrawStr;

    if (message_request[2] == "MY")
    {
        qDebug() << "Update my field: " << message_request[3];
//        qDebug() << "MYYYY";
        updateMyFieldDraw(fieldDrawStr);
    }
    else if (message_request[2] == "ENEMY")
    {
        qDebug() << "Update enemy field: " << message_request[3];
//        qDebug() << "ENEMYYY";
        updateEnemyFieldDraw(fieldDrawStr);
    }
    else
    {
        qDebug() << "Wrong request";
    }
}

void MainWindow::handleHistoryUpdateRequest()
{
    QStringList gameEndingsStrList = QString::fromUtf8(data_.mid(15)).split("$$");
//    qDebug() << gameEndingsStrList;

    if (gameEndingsStrList.size() == 0)
    {
        qDebug() << "No saved games";
        return;
    }

    for (int i = 0; i < gameEndingsStrList.size(); i++)
    {
        qDebug() << gameEndingsStrList[i];
    }

    fightsHistoryWindow_.fillTable(gameEndingsStrList);
}

void MainWindow::handleExitRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");

    if (message_request.size() < 2)
    {
        qDebug() << "wrong request";
        return;
    }

    QString login_exited = message_request[1];

    // TODO: remove chat with user and user from the list
    QList<QListWidgetItem*> exited_list = ui->messageRecieversOptionList->findItems(login_exited, Qt::MatchExactly);

    if (exited_list.isEmpty())
    {
        qDebug() << "No such chat in messageRecieversOptionList";
        return;
    }

    QListWidgetItem* exited_user = exited_list.first();  // specific sender from the messageRecieversOptionList
    QTextBrowser* chat = browserMap.value(exited_user);    // look for specific chat

    if (!chat)
    {
            qDebug() << "Error: no such chat in browserMap";
            return;
    }

//    ui->usersList;

    qDebug() << "deleting chat with exited user " << login_exited;

    // TODO:
//    ui->messageRecieversOptionList->removeItemWidget(exited_user);
//    browserMap.remove(exited_user);
}

void MainWindow::handleConnectionRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");
    QString enemy_login = message_request[1];

    if (message_request.size() == 2)    // CONNECTION:<login1>
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Запрос на подключение",
                                                                  "Пользователь " + enemy_login + " приглашает вас сыграть! Принять приглашение?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        QString answer = "CONNECTION:" + enemy_login + ":";

        if (reply == QMessageBox::Yes)
        {
            qDebug() << "You have accepted the invitation from " << enemy_login;

            // проверка на то,что сообщение дошло? хз

            answer += "ACCEPT";
        }
        else
        {
            qDebug() << "You haven't accepted the invitation from " << enemy_login;

            answer += "REJECT";
        }

        qDebug() << answer;
        socket_->write(((QString)answer + "@").toUtf8());
//        socket_->flush();

        model_->setStartedFlag(false);
    }

    else if (message_request.size() == 3)   // CONNECTION:<login2>:ACCEPT/REJECT
    {
        QString resultOfConnection = message_request[2];

        if (resultOfConnection == "ACCEPT")
        {
            QMessageBox::information(this, "Connection info!", "Пользователь " + enemy_login + " принял запрос на игру!");

            QString message = "GAME:START:" + login_ + ":" + enemy_login;

            socket_->write((message+"@").toUtf8());
//            socket_->flush();
            qDebug() << message;

            qDebug() << "Connection to " << enemy_login << " done!";

            model_->setStartedFlag(true);

            return;
        }

        else if (resultOfConnection == "REJECT")
        {
            QMessageBox::information(this, "Connection info!", "Пользователь " + enemy_login + " отклонил запрос на игру!");
            qDebug() << "Didn't connect to " << enemy_login;
        }

        else
                qDebug() << "Wrong request";
    }

    else
        qDebug() << "Wrong request";
}

void MainWindow::handleGameRequest()
{
    QStringList message_request = QString::fromUtf8(data_).split(":");
    qDebug() << message_request;

    if (message_request.size() == 3)
    {
        if (message_request[1] == "FINISH") // GAME:FINISH:<login>
        {
            QString winnerLogin = message_request[2];

            if (winnerLogin == login_)
            {
                QString message = "Вы победили!";
                controller_->playSound("victory_sound");
                QMessageBox::information(this, "Information!", message);
                controller_->stopSound("victory_sound");
                finishGame();
            }
            else if (winnerLogin == model_->getEnemyLogin())
            {
                QString message = "Игрок " + winnerLogin + " победил!";
                controller_->playSound("defeat_sound");
                QMessageBox::information(this, "Information!", message);
                controller_->stopSound("defeat_sound");
                finishGame();
            }
            else
            {
                qDebug() << "Wrong request!";
            }
        }
    }

    if (message_request.size() == 2)
    {
        if (message_request[1] == "STOP") // GAME:STOP
        {
            QMessageBox::information(this, "Information!", "Игра остановлена одним из пользователей...");
            finishGame();
        }

        else if (message_request[1] == "FIGHT") // GAME:FIGHT
        {
            qDebug() << "Starting fight?";
            startFight();
        }

        else
            qDebug() <<"Wrong request";
    }

    else if (message_request.size() == 4)   // GAME:START:<enemy_login>:<gameId>
    {
        if (message_request[1] == "START")
        {
            QString enemy_login = message_request[2];
            int gameId = message_request[3].toInt();

            startGame(enemy_login, gameId);
        }
        else
            qDebug() << "Wrong request";
    }

    else
        qDebug() << "Wrong request";
}

static QString convertBinFieldToState(const QString& fieldStr)
{
    QString fieldStateStr = "";

    for (QString::const_iterator it = fieldStr.begin(); it != fieldStr.end(); ++it)
    {
        if (it->digitValue() == 0)
            fieldStateStr += QString::number((int)CL_ST_EMPTY);

        else if (it->digitValue() == 1)
            fieldStateStr += QString::number((int)CL_ST_UNDEFINED);

        else
            qDebug() << "Wrong fieldStr!";
    }

    return fieldStateStr;
}

void MainWindow::handleGenerateRequest()    // GENERATE:<fieldBin>
{
    ModelState state = model_->getState() ;

    if (state == ST_MAKING_STEP ||
        state == ST_WAITING_STEP  )
        return;

    QStringList message_request = QString::fromUtf8(data_).split(":");

    if (message_request.size() < 2)
    {
        qDebug() << "Wrong answer from server";
        ui->applyIsOkLabel->setVisible(false);
        ui->applyIsNotOkLabel->setVisible(true);
        return;
    }

    ui->applyIsOkLabel->setVisible(true);
    ui->applyIsNotOkLabel->setVisible(false);

    QString fieldBinStr = message_request[1];
    qDebug() << "Server generated a field: " << fieldBinStr;

    QString fieldStr = convertBinFieldToState(fieldBinStr);
    qDebug() << "Converted field: " << fieldStr;

    model_->myField_->setStateField(fieldStr);
    model_->myField_->initMyDrawField();
}

void MainWindow::on_loginLabel_cursorPositionChanged(int , int )
{

}

void MainWindow::screenUpdate()
{

}

void MainWindow::updateAll()
{
    connectionStateUpdate(connectionState_);
    makeUsersRequest();
    screenUpdate();
}

void MainWindow::on_updateButton_clicked()
{
    CLICK_SOUND
    updateAll();
//    socket_->write(((QString)"UPDATE:").toUtf8());
}

void MainWindow::sendMessage()
{
    QString message = ui->messageEdit->toPlainText();

    if (message.isEmpty())
    {
        qDebug() << "empty message";
        return;
    }

    QListWidgetItem* currentItem = ui->messageRecieversOptionList->currentItem();

    if (currentItem)
    {
        QString receiver_login = currentItem->text();
        qDebug() << "Sending message to:" << receiver_login;

        QString request = "MESSAGE:" + receiver_login + ":" + message;  // format:  MESSAGE:<receiver_login>:message
        qDebug() << request;

        QTextBrowser* chat = browserMap.value(ui->messageRecieversOptionList->findItems(receiver_login, Qt::MatchExactly).first());    // choose chat with the receiver

        if (!chat)
        {
            qDebug() << "Chat not found";
            return;
        }

//        chat->append(login_ + "< " + message);    // show sended message
//        qDebug() << login_ + "< " + message ;

        QTextCharFormat sender_format;
        QTextCharFormat message_format;

        sender_format.setForeground(QColor("blue"));
        message_format.setForeground(QColor("black"));

        chat->textCursor().insertText(login_ + "> ", sender_format);
        chat->textCursor().insertText(message + "\n", message_format);

        socket_->write((request+"@").toUtf8());   // send message through the server
//        socket_->flush();
        ui->messageEdit->clear();
    }
    else
    {
        QMessageBox::warning(this, "ERROR!", "No reciever selected");
        qDebug() << "No reciever selected";
    }
}

void MainWindow::on_sendMessageButton_clicked()
{
    CLICK_SOUND
    sendMessage();
}

void MainWindow::connectToGame(const QString& enemy_login)
{
    socket_->write(((QString)"CONNECTION:" + enemy_login + "@").toUtf8());
//    socket_->flush();
    qDebug() << "CONNECTION:" << enemy_login;
}

void MainWindow::on_messageRecieversOptionList_itemSelectionChanged()
{
    QListWidgetItem* receiver = ui->messageRecieversOptionList->currentItem();

    if (receiver->data(Qt::UserRole).toBool())  // if had new unread messages
    {
//        receiver->setCheckState(Qt::CheckState::Checked);
        receiver->setBackground(QBrush(QColor("white")));
        receiver->setForeground(QBrush(QColor("black")));
        receiver->setData(Qt::UserRole, true);
    }

    QTextBrowser* browser = browserMap.value(receiver);
    qDebug() << login_<< " + " << receiver->text() << " - chat selected";

    if (!browser)
    {
//        qDebug() << "No such chat!";
        createTextBrowser(receiver);
    }

    int index = receiverBrowserStackedWidget->indexOf(browser);
    receiverBrowserStackedWidget->setCurrentIndex(index);   // set corresponding browser widget
}

void MainWindow::createTextBrowser(QListWidgetItem* receiver)
{
    QTextBrowser* browser = new QTextBrowser;
    browser->setVisible(false);
//    browser->setText(QString("Chat for %1 and %2").arg(login_, receiver->text()));
    receiverBrowserStackedWidget->addWidget(browser);
    browserMap.insert(receiver, browser);
    qDebug() << "Create chat for " << login_ << " and " << receiver->text();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    static int n_try = 0;
    n_try++;

    if (n_try > 30)
    {
        stopClient("Timeout. Cannot connect to the server...");
        qDebug() << "after " << n_try << " tries client cannot connect to the server";
    }

    if (connectionState_ == ST_DISCONNECTED)
    {
        connectUser();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    pos.setY(pos.y() - ui->fieldsLabel->y());
    pos.setX(pos.x() - ui->fieldsLabel->x());
    controller_->onMousePressed(pos, event, ui->applyIsOkLabel, ui->applyIsNotOkLabel, ui->applyFieldButton);

    qDebug() << "Clicked on: " << pos;

    event->accept();
}

void MainWindow::paintEvent(QPaintEvent* event) // calls when interface redraws
{
    Q_UNUSED(event);

    if (ui->fieldsLabel)
    {
        QImage image = pictures.get("background");
        QImage myFieldDrawImage    = model_->getMyField().getFieldImage();
        QImage enemyFieldDrawImage = model_->getEnemyField().getFieldImage();

        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.drawImage(MYFIELD_IMG_REL_X   , MYFIELD_IMG_REL_Y   , myFieldDrawImage   );
        painter.drawImage(ENEMYFIELD_IMG_REL_X, ENEMYFIELD_IMG_REL_Y, enemyFieldDrawImage);

        painter.end();

        ui->fieldsLabel->setPixmap(QPixmap::fromImage(image));
    }

    event->accept();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "exit from the programm";

    if (connectionState_ != ST_DISCONNECTED)
        exitFromServer();

    event->accept();
}

void MainWindow::exitFromServer()
{
    socket_->write(((QString)"EXIT:@").toUtf8());    // send server message about user disconnect
//    socket_->flush();
    qDebug() << "EXIT:";

    socket_->close();
}

void MainWindow::stopClient(QString msg)
{
    QMessageBox::information(this, "information!", msg);
    this->close();
}

void MainWindow::startGame(QString enemy_login, int gameId)
{
    model_->startGame(enemy_login, gameId);
//    controller_->startGame(enemy_login, gameId);

    ui->myGameLoginLabel->setText(login_);
    ui->enemyGameLoginLabel->setText(enemy_login);

    ui->gameExitButton->setVisible(     true);
    ui->applyFieldButton->setVisible(   true);
    ui->generateFieldButton->setVisible(true);
    ui->isReadyCheckBox->setEnabled(false);

    ui->checkButton->setEnabled(true);
    ui->generateFieldButton->setEnabled(true);
    ui->applyFieldButton->setEnabled(true);
    ui->clearButton->setEnabled(true);

    ui->applyFieldButton->setStyleSheet("");

    updateReadiness(ST_PLAYING);
    controller_->stopSound("intro_music");
    controller_->playSound("field_music");


    // TODO: ...
}

void MainWindow::finishGame()
{
    model_->finishGame();
//    controller_->finishGame(enemy_login);

    ui->myGameLoginLabel->clear();
    ui->enemyGameLoginLabel->clear();

    ui->gameExitButton->setVisible(     false);
    ui->applyFieldButton->setVisible(   false);
    ui->generateFieldButton->setVisible(true );
    ui->checkButton->setVisible(true );
    ui->clearButton->setVisible(true);
    ui->applyIsNotOkLabel->setVisible(false);
    ui->applyIsOkLabel->setVisible(false);
    ui->isReadyCheckBox->setEnabled(true);

    ui->generateFieldButton->setEnabled(true);
    ui->checkButton->setEnabled(true);
    ui->clearButton->setEnabled(true);

    ui->whooseStepLabel->setVisible(false);

    updateReadiness(ST_NREADY);
    controller_->playSound("field_music");
}

void MainWindow::startFight()
{
    model_->startFight();
    qDebug() << "Fight started!";

    controller_->playSound("background");

    ui->applyFieldButton->setVisible(false);
    ui->generateFieldButton->setVisible(false);
    ui->checkButton->setVisible(false);
    ui->clearButton->setVisible(false);
    ui->applyIsOkLabel->setVisible(false);
    ui->applyIsNotOkLabel->setVisible(false);
    ui->whooseStepLabel->setVisible(true);

    if (model_->getStartedFlag())   // if I started the game
    {
        ui->whooseStepLabel->setText("Ваш ход");
    }
    else
    {
        ui->whooseStepLabel->setText("Ход соперника");
    }
}

void MainWindow::on_gameExitButton_clicked()
{
    CLICK_SOUND
    if (model_->getState() == ST_GAME_NSTARTED ||
        model_->getState() == ST_GAME_FINISHED   )
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Завершить игру",
                                                              "Хотите завершить игру досрочно?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        qDebug() << "Don't want to stop the game";
        return;
    }

    qDebug() << "You want to stop the current game";

    int gameId = model_->getGameId(); // TODO: get gameId;
    QString message = "GAME:" + QString::number(gameId) + ":FINISH";

    socket_->write((message+"@").toUtf8());  // GAME:<gameId:FINISH
//    socket_->flush();
    qDebug() << message;
}

//void MainWindow::on_checkButton_clicked()
//{
//    qDebug() << "My field: " << model_->getMyFieldStr();

//    bool is_correct = model_->isMyFieldCorrect();

//    QString result_msg = "Result of check: ";
//    if (is_correct)
//        result_msg += "CORRECT";
//    else
//        result_msg += "INCORRECT";

//    QMessageBox::information(this, "IS CORRECT? INFO", result_msg);
//    qDebug() << "Result of check: " << is_correct;
//}


void MainWindow::on_generateFieldButton_clicked()
{
    CLICK_SOUND
    ModelState state = model_->getState();
    if (state == ST_MAKING_STEP  ||
        state == ST_WAITING_STEP ||
        state == ST_WAITING_PLACING)
        return;

    QString message = "GENERATE:";
    socket_->write((message+"@").toUtf8());
//    socket_->flush();

//    model_->generateMyField();
//    ui->centralwidget->update();
}

void MainWindow::on_applyFieldButton_clicked()
{
    CLICK_SOUND
    if (!model_->isMyFieldCorrect())
    {
//        model_->updateState(ST_WAITING_PLACING);
        qDebug() << "Incorrect ship placing";
        QMessageBox::warning(this, "Ship placing warning", "Расстановка кораблей некорректна! Поменяйте её");
        ui->applyFieldButton->setStyleSheet("");
        return;
    }

    qDebug() << "Ship placement is correct! Sending to a server)";

    QString message = "GAME:" + QString::number(model_->getGameId()) + ":" + login_ + ":FIELD:" + model_->getMyFieldStr();
    socket_->write((message+"@").toUtf8());
//    socket_->flush();
    qDebug() << message;

    model_->updateState(ST_WAITING_PLACING);
    ui->applyFieldButton->setStyleSheet("background-color: rgb(143, 240, 164);");

    ui->checkButton->setEnabled(false);
    ui->generateFieldButton->setEnabled(false);
    ui->applyFieldButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    ui->applyIsOkLabel->setVisible(true);
    ui->applyIsNotOkLabel->setVisible(false);
}


void MainWindow::on_clearButton_clicked()
{
    CLICK_SOUND
    ModelState state = model_->getState();
    if (state == ST_MAKING_STEP  ||
        state == ST_WAITING_STEP ||
        state == ST_WAITING_PLACING)
        return;

    model_->clearMyField();
}

void MainWindow::on_checkButton_clicked()
{
    CLICK_SOUND
    qDebug() << "My field: " << model_->getMyFieldStr();

    bool is_correct = model_->isMyFieldCorrect();

    QString result_msg = "Результат проверки вашей расстановки: ";
    if (is_correct)
        result_msg += "КОРРЕКТНАЯ";
    else
        result_msg += "НЕКОРРЕКТНАЯ";

    QMessageBox::information(this, "IS CORRECT? INFO", result_msg);
    qDebug() << "Result of check: " << is_correct;
}

void MainWindow::on_isReadyCheckBox_stateChanged(int state)
{
    CLICK_SOUND

    if (state == Qt::Checked)
    {
        updateReadiness(ST_READY);
    }
    else if (state == Qt::Unchecked)
    {
        updateReadiness(ST_NREADY);
    }
}

void MainWindow::updateReadiness(MainWindow::Readiness readiness)
{
    if (readiness == ST_NREADY)
    {
        qDebug() << "user is not ready!";
        ui->isReadyCheckBox->setStyleSheet("QCheckBox { color: black; }"
                                       "QCheckBox::indicator { background-color: yellow; }");
        ui->isReadyCheckBox->setText("не готов");
    }
    else if (readiness == ST_READY)
    {
        qDebug() << "user is ready!";
        ui->isReadyCheckBox->setStyleSheet("QCheckBox { color: black; }"
                                       "QCheckBox::indicator { background-color: green; }");
        ui->isReadyCheckBox->setText("готов");
    }
    else if (readiness == ST_PLAYING)
    {
        qDebug() << "user is in a game!";
        ui->isReadyCheckBox->setStyleSheet("QCheckBox { color: black; }"
                                       "QCheckBox::indicator { background-color: blue; }");
        ui->isReadyCheckBox->setText("в игре");
    }
    else
    {

    }

    readiness_ = readiness;

    QString message = "READINESS:" + QString::number(readiness_);
    socket_->write((message+"@").toUtf8());
//    socket_->flush();
    qDebug() << message;
}

void MainWindow::on_volumeSlider_actionTriggered(int action)
{
//    qDebug() << (ui->volumeSlider->value();
    int volume = (ui->volumeSlider->value()-5) % 101; // * 100.0) / ui->volumeSlider->maximum();
    qDebug() << "volume is " << volume << "%";
    controller_->updateVolume(volume);
}

void MainWindow::updateMyFieldDraw(QString fieldDrawStr)
{
    QVector<CellDraw> fieldDraw = fieldDrawFromStr(fieldDrawStr);
    model_->updateMyFieldDraw(fieldDraw);
}

void MainWindow::updateEnemyFieldDraw(QString fieldDrawStr)
{
    QVector<CellDraw> fieldDraw = fieldDrawFromStr(fieldDrawStr);
    model_->updateEnemyFieldDraw(fieldDraw);
}
