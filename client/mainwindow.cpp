#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QTextBrowser>

MainWindow::MainWindow(QString ip, quint16 port, QWidget *parent)
    : QMainWindow(parent)
    , ip_(ip)
    , port_(port)
    , ui(new Ui::MainWindow)
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

    connect(ui->messageRecieversOptionList, SIGNAL(itemSelectionChanged()), this, SLOT(on_messageRecieversOptionList_itemSelectionChanged()));

    socket_ = new QTcpSocket(this);

    userLogins_ = {}; // list of user logins

    stateUpdate(ST_DISCONNECTED);

    connect(socket_, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(on_sockError(QAbstractSocket::SocketError)));  // handles socket errors
    connect(socket_, SIGNAL(connected())   , this, SLOT(on_sockConnect())   );  // when socket connected, sockConnect slot runs
    connect(socket_, SIGNAL(readyRead())   , this, SLOT(on_receiveData())     );  // when new network data comes, sockReady slot runs
    connect(socket_, SIGNAL(disconnected()), this, SLOT(on_sockDisconnect()));  // when socket disconnected, sockDisconnect slot runs
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stateUpdate(ClientState new_state)
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
        case ST_MAKING_STEP:
        {
            ui->statusbar->showMessage("status: ST_MAKING_STEP");
            break;
        }
        case ST_WAITING_STEP:
        {
            ui->statusbar->showMessage("status: ST_WAITING_STEP");
            break;
        }
        case ST_READY:
        {
            ui->statusbar->showMessage("status: ST_READY");
            break;
        }
    }

    state_ = new_state;
}

void MainWindow::screenUpdate()
{

}

void MainWindow::on_connectToServerButton_clicked()    // connection+authorization button
{
    if (state_ == ST_DISCONNECTED)
    {
        qDebug() << "Connect button pushed";
        socket_->connectToHost(ip_, port_);

        if (!socket_->waitForConnected(2500))
        {
            qDebug() << "Cannot connect";
            QMessageBox::warning(this, "ERROR!", "Cannot connect user to server...");
            return;
        }

        stateUpdate(ST_CONNECTED);
        ui->connectToServerButton->setText("Авторизоваться на сервере");
//        qDebug() << "Connected";
    }

    if (state_ == ST_CONNECTED)
    {
        QString login_entered = ui->loginLabel->text();

        if (!login_entered.isEmpty())
        {
            qDebug() << "trying to connect to server with login " << login_entered << "";

            socket_->write(("AUTH:" + login_entered).toUtf8()); // request for authorization

            if (!socket_->waitForReadyRead(2500)) // if server don't answer > 2.5 sec
            {
                qDebug() << "Server don't answer";
                return;
            }

            QString answer = QString::fromUtf8(data_).trimmed();    // gettin g server answer
            if (answer.startsWith("AUTH:SUCCESS"))
            {
                stateUpdate(ST_AUTHORIZED);
                login_ = login_entered;

                ui->loginLabel->setReadOnly(true); // block loginLabel for writing after user if authorized

    //            QString login = login_entered.trimmed();
    //            ui->usersList->addAction(login);

                ui->loginLabel->setStyleSheet("background-color: green;\n color: white;");

                qDebug() << "loggining confirmed!";

                ui->connectToServerButton->setText("Вы успешно авторизованы!");
                ui->connectToServerButton->setEnabled(false);   // turn the button off

                // TODO: get list of users from server
                usersListUpdate();
            }

             else // didn't authorized
            {
                ui->loginLabel->setStyleSheet("background-color: red;\n color: white;");
                QMessageBox::warning(this, "ERROR!", "Login is already in use");
            }
        }
    }

//    if (state_ == ST_AUTHORIZED)
//        return;
}

void MainWindow::on_sockConnect()
{

}

void MainWindow::on_sockDisconnect()
{
    socket_->deleteLater();
}

void MainWindow::on_sockError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error " << error;
}

void MainWindow::on_receiveData()
{
//    socket->waitForReadyRead(500);
    data_ = socket_->readAll();
    qDebug() << "server: " << data_;

    handleData();
}

void MainWindow::handleData()
{
    if (data_.startsWith("MESSAGE"))
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

        chat->append(sender_login + "> " + message);    // show received message in common chat (all)

        if (ui->messageRecieversOptionList->currentItem() != sender)   // if message to not a current chat then we set a flag to unchecked (then change the color)
        {
            sender->setBackground(QBrush(QColor("green")));
            sender->setForeground(QBrush(QColor("white")));
            sender->setData(Qt::UserRole, true); // new flag means about new message not read yet
        }
    }

    else if(data_.startsWith("USERS"))
    {
        handleUsersListUpdate();
    }

    else
    {

    }
}

void MainWindow::usersListUpdate() // requests list of users and add connected/remove disconnected them in usersList and in messageRecieversOptionList
{
    socket_->write(((QString)"USERS:").toUtf8());
    socket_->waitForReadyRead(500);

    handleUsersListUpdate();
}

// TODO: create a method that updates (creates/removes) all chat QTextBrowser's

void MainWindow::handleUsersListUpdate()
{
//    qDebug() << "HANDLE";

    QStringList logins = QString::fromUtf8(data_.trimmed().mid(6)).split(" ", Qt::SkipEmptyParts); // getting list of string logins from the request
    QString cur_login = "";

    QListWidgetItem* cur_user = ui->messageRecieversOptionList->currentItem();
    if (cur_user)
        cur_login = cur_user->text();

    ui->usersList->clear();
    ui->messageRecieversOptionList->clear();

    ui->messageRecieversOptionList->addItem("all");
    int cur_row_index = 0;
    int new_cur_row_index = cur_row_index;

    foreach (const QString& login, logins)
    {
        ui->usersList->addAction(login);
        ui->messageRecieversOptionList->addItem(login);
        cur_row_index++;

        qDebug() << "User " << login;

        if (cur_login == login)
            new_cur_row_index = cur_row_index;

//        // TODO: add smarter user chats update, because char now are removed and then created again
    }

    ui->messageRecieversOptionList->setCurrentRow(new_cur_row_index);   // set message to all at default

//    updateChats();
}

void MainWindow::on_loginLabel_cursorPositionChanged(int , int )
{

}

void MainWindow::updateAll()
{
    stateUpdate(state_);
    usersListUpdate();
    screenUpdate();
}

void MainWindow::on_updateButton_clicked()
{
    updateAll();
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

        chat->append(login_ + "< " + message);    // show sended message
        qDebug() << login_ + "< " + message ;

        socket_->write(request.toUtf8());   // send message through the server
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
    sendMessage();
}

void MainWindow::updateChats()
{
    QList<QListWidgetItem*> receiversList;
    foreach (QListWidgetItem* receiver, receiversList)
    {
        QTextBrowser* chat = browserMap.value(receiver);

        if (!chat)
        {
            createTextBrowser(receiver);
        }
    }

    // TODO: add remove chats function
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

