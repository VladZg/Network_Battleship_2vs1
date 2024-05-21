#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "config.hpp"
#include "client.hpp"
#include <QTimer>
#include <QDateTime>

class GameController
{
public:
    enum GameState
    {
        ST_NSTARTED =  0,   // игра не начата
        ST_PLACING      ,   // пользователи располагают корабли
        ST_STARTED_STEP ,   // ход пользователя, начавшего игру
        ST_ACCEPTED_STEP,   // ход пользователя, принявшего приглашение
        ST_FINISHED     ,   // игра завершена
    };

    GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted);
    ~GameController();

    ClientsIterator getClientStartedIt();
    ClientsIterator getClientAcceptedIt();
    int getGameId();
    GameState getState();
    void updateState(GameController::GameState state);

    void incNPlaced();
    int getNPlaced();
    void incNDamaged(bool isStartedDamaged);
    bool checkGameFinish(bool isStartedKilled);

    void setClientStartedField(QString field);
    void setClientAcceptedField(QString field);
    void setClientStartedFieldState(QString field);
    void setClientAcceptedFieldState(QString field);

public:
    QString winnerLogin_;

    QDate startDate_;
    QDateTime startTime_;

    QDate endDate_;
    QDateTime endTime_;

private:
    int gameId_;
    GameState state_;
    int nPlaced_;
    int nAcceptedDamaged_;
    int nStartedDamaged_;
    int nDecks_;

    ClientsIterator clientStarted_;     // client who started  the game
    ClientsIterator clientAccepted_;    // client who accepted the game
    Field clientStartedField_;
    Field clientAcceptedField_;
};

typedef QMap<int, GameController> Games;
typedef Games::iterator GamesIterator;

#endif // GAMECONTROLLER_H
