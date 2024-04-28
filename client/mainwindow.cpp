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
    chatWidgetLayout->addWidget(ui->messageRecieversOptionList);
    chatWidgetLayout->addWidget(receiverBrowserStackedWidget);
    ui->chatWidget->setLayout(chatWidgetLayout);

    connect(ui->messageRecieversOptionList, SIGNAL(itemSelectionChanged()), this, SLOT(on_messageRecieversOptionList_itemSelectionChanged()));

    socket_ = new QTcpSocket(this);
    is_logined = false;
    is_connected = false;

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

void MainWindow::on_connectToServerButton_clicked()    // authorization button
{
    if (is_logined)
        return;

    if (!is_connected)
    {
        qDebug() << "Connect button pushed";
        socket_->connectToHost(ip_, port_);

        if (!socket_->waitForConnected(2500))
        {
            qDebug() << "Cannot connect";
            QMessageBox::warning(this, "ERROR!", "Cannot connect user to server...");
            return;
        }

        is_connected = true;
//        qDebug() << "Connected";
    }

    ui->connectToServerButton->setText("Авторизоваться на сервере");
    stateUpdate(ST_CONNECTED);

    QString login_entered = ui->loginLabel->text();

    if (!login_entered.isEmpty())
    {
        qDebug() << "trying to connect to server with login " << login_entered << "";

        socket_->write(("AUTH " + login_entered).toUtf8());

        // TODO: check if login is already used
        // if (is_available) return;

        if (!socket_->waitForReadyRead(2500)) // if server don't answer > 2.5 sec
        {
            qDebug() << "Server don't answer";
            return;
        }

        QString answer = QString::fromUtf8(data_).trimmed();
        if (answer.startsWith("AUTH_SUCCESS"))
        {
            is_logined = true;
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

         else // didn't connected
        {
            ui->loginLabel->setStyleSheet("background-color: red;\n color: white;");
            QMessageBox::warning(this, "ERROR!", "Login is already in use");
        }
    }
    else
    {
        QMessageBox::warning(this, "ERROR!", "Cannot authorize user on server...");
    }
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
    if (data_.startsWith("ALL"))
    {
        QStringList message_request = QString::fromUtf8(data_).split(":");
        QString sender_login = message_request[1];
        QString message = message_request[2];

        if (sender_login == login_) // if message from myself
            return;

        QList<QListWidgetItem *> sendersList = ui->messageRecieversOptionList->findItems("all", Qt::MatchContains);

        if (sendersList.isEmpty())
        {
            qDebug() << "No all in the list";
            return;
        }

        QListWidgetItem* sender = sendersList.first();  // specific sender from the messageRecieversOptionList
//        qDebug() << sender;
        QTextBrowser* chat = browserMap.value(sender);    // look for specific chat

        if (!chat)
        {
            qDebug() << "Error";
            return;
        }

//        QTextBrowser* chat = browserMap.value(ui->messageRecieversOptionList->findItems("all", Qt::MatchContains).first());
        chat->append(sender_login + "> " + message);    // show received message in common chat (all)
    }
    else if (data_.startsWith("MESSAGE_FROM"))   // NOT IMPLEMENTED YET
    {
//        QStringList message_request = QString::fromUtf8(data_).split(":");
//        QString sender_login = message_request[1];
//        QString message = message_request[2];

//        if (sender_login == login_) // if message from myself
//            return;

//        QList<QListWidgetItem *> sendersList = ui->messageRecieversOptionList->findItems(sender_login, Qt::MatchContains);
//        qDebug() << ui->messageRecieversOptionList << "\n" << sendersList << "\n" << sender_login;

//        if (sendersList.isEmpty())
//        {
//            qDebug() << "No user with this login in the list";
//            return;
//        }

//        QListWidgetItem* sender = sendersList.first();  // specific sender from the messageRecieversOptionList
//        qDebug() << sender;
//        QTextBrowser* chat = browserMap.value(sender);    // look for specific chat

//        if (!chat)
//        {
//            qDebug() << "Error";
//            return;
//        }

//        QTextBrowser* chat = browserMap.value(ui->messageRecieversOptionList->findItems("all", Qt::MatchContains).first());
//        chat->append(sender_login + "> " + message);    // show received message
    }
}

void MainWindow::usersListUpdate() // requests list of users and add connected/remove disconnected them in usersList and in messageRecieversOptionList
{
    socket_->write(((QString)"USERS_LIST ").toUtf8());
    socket_->waitForReadyRead(500);

    QStringList logins = QString::fromUtf8(data_.trimmed().mid(11)).split(" ", Qt::SkipEmptyParts); // getting list of string logins from the request

    ui->usersList->clear();

    ui->messageRecieversOptionList->clear();
    ui->messageRecieversOptionList->addItem("all");
    ui->messageRecieversOptionList->setCurrentRow(0);   // set message to all at default

    foreach (const QString& login, logins)
    {
        ui->usersList->addAction(login);
        ui->messageRecieversOptionList->addItem(login);

        qDebug() << "User " << login;

        // TODO: add smarter check MESSAGE_FROM
    }

//    qDebug() << ui->usersList->actions();
}

// TODO: create a method that updates (creates/removes) all chat QTextBrowser's

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

//        if (receiver_login == login_)
//            return;

        QString request = "MESSAGE_TO:" + receiver_login + ":" + message;  // format:  MESSAGE_TO:receiver_login:message
        qDebug() << request;

        // now it doesn't work if you want to send messages in private chat
        if (receiver_login != "all")
        {
            qDebug() << "NOT IMPLEMENTED FEATURE YET";
            return;

            //        QList<QListWidgetItem *> recieversList = ui->messageRecieversOptionList->findItems(receiver_login, Qt::MatchContains);
            //        qDebug() << ui->messageRecieversOptionList << recieversList << receiver_login;

            //        if (recieversList.isEmpty())
            //        {
            //            qDebug() << "No user with this login in the list";
            //            return;
            //        }

            //        QListWidgetItem* reciever = recieversList.first();  // specific sender from the messageRecieversOptionList
            //        QTextBrowser* chat = browserMap.value(reciever);    // look for specific chat
        }

        QTextBrowser* chat = browserMap.value(ui->messageRecieversOptionList->findItems("all", Qt::MatchContains).first());
        chat->append(login_ + "< " + message);    // show sended message

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

void MainWindow::on_messageRecieversOptionList_itemSelectionChanged()
{
    QListWidgetItem* receiver = ui->messageRecieversOptionList->currentItem();
    QTextBrowser* browser = browserMap.value(receiver);

    if (!browser)
    {
        createTextBrowser(receiver);
//        browser = browserMap.value(receiver);
    }

    int index = receiver->listWidget()->row(receiver);
    receiverBrowserStackedWidget->setCurrentIndex(index);   // set corresponding browser widget
}

void MainWindow::createTextBrowser(QListWidgetItem* receiver)
{
    QTextBrowser* browser = new QTextBrowser;
    browser->setVisible(false);
//    browser->setText(QString("Chat for %1 and %2").arg(login_, receiver->text()));
    receiverBrowserStackedWidget->addWidget(browser);
    browserMap.insert(receiver, browser);
}

