#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QImage>
#include <QStringList>
#include <QListWidget>
#include <QTextBrowser>
#include <QStackedWidget>
#include <QPainter>
#include <QMediaPlayer>
#include "config.hpp"
#include "constants.hpp"
#include "field.hpp"
#include "model.hpp"
#include "controller.hpp"
#include "fightshistorywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum ClientConnectionState  // connection states of client
{
    ST_DISCONNECTED = 0,
    ST_CONNECTED       ,
    ST_AUTHORIZED      ,
    // more
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString ip, quint16 port, QWidget *parent = nullptr);
    ~MainWindow();
    void screenUpdate();
    void connectionStateUpdate(ClientConnectionState new_state);
    void modelStateUpdate(ModelState new_state);
    void updateAll();
    void exitFromServer();

//    FightsHistoryWindow fightsHistoryWindow_;
    FightsHistoryWindow fightsHistoryWindow_;
    QStringList users_;

private:
    ClientConnectionState connectionState_;
    QString ip_;
    int port_;
    QTcpSocket* socket_; // socket for interacting with server
    QByteArray data_;    // data from socket and for socket
    QString login_;
    Model* model_;
    int timerId_;
    Controller* controller_;

protected:
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

public:
    QStringList userLogins_;
    void connectUser();
    void authenticateUser();
    void handleData();
    void makeUsersRequest();
    void updateUsers(QStringList users_list);
    void sendMessage();
    void connectToGame(const QString& enemy_login);
    void handleMessageRequest();
    void handleShotRequest();
    void handleUsersRequest();
    void handlePingRequest();
    void handleFieldRequest();
    void handleHistoryUpdateRequest();
    void handleExitRequest();
    void handleConnectionRequest();
    void handleGameRequest();
    void handleGenerateRequest();
    void updateChats();
    void stopClient(QString msg);

    void updateMyFieldDraw(QString fieldDrawStr);
    void updateEnemyFieldDraw(QString fieldDrawStr);

    void setIconStatus(QAction* userToChoose, int readiness);

    void startFight();
    void startGame(QString enemy_login, int gameId);
    void finishGame();

    QPixmap* getFieldImage(const Field& field);

public slots:
    void on_sockConnect();
    void on_receiveData();
    void on_sockDisconnect();
    void on_sockError(QAbstractSocket::SocketError error);
    void on_userToChose_triggered();
    void on_openFightHistoryAction_triggered();

private slots:
    void on_connectToServerButton_clicked();
    void on_loginLabel_cursorPositionChanged(int arg1, int arg2);
    void on_updateButton_clicked();
    void on_sendMessageButton_clicked();
    void on_messageRecieversOptionList_itemSelectionChanged();
    void createTextBrowser(QListWidgetItem* user);

    void on_gameExitButton_clicked();
    void on_generateFieldButton_clicked();
    void on_applyFieldButton_clicked();
    void on_clearButton_clicked();
    void on_checkButton_clicked();
    void on_isReadyCheckBox_stateChanged(int arg1);
    void on_volumeSlider_actionTriggered(int action);

private:
    QListWidget* receiversListWidget;    // user from the list who we want to send the message
    QStackedWidget* receiverBrowserStackedWidget;  // browser (chat) with this user
    QMap<QListWidgetItem*, QTextBrowser*> browserMap;   // map of the user logins and chats with them

public:
    enum Readiness
    {
        ST_NREADY       = 0,
        ST_READY           ,
        ST_PLAYING         ,
    };

    void updateReadiness(MainWindow::Readiness readiness);

private:
    Readiness readiness_;

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
