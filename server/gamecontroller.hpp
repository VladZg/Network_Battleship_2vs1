#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "config.hpp"
#include "client.hpp"

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

    void incNPlaced();
    int getNPlaced();

    void setClientStartedField(QString field);
    void setClientAcceptedField(QString field);
    void setClientStartedFieldState(QString field);
    void setClientAcceptedFieldState(QString field);

private:
    int gameId_;
    GameState state_;
    int nPlaced_;

    ClientsIterator clientStarted_;     // client who started  the game
    ClientsIterator clientAccepted_;    // client who accepted the game
    Field clientStartedField_;
    Field clientAcceptedField_;
};

typedef QMap<int, GameController> Games;
typedef Games::iterator GamesIterator;

#endif // GAMECONTROLLER_H
