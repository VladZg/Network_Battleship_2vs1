#include "gamecontroller.h"

GameController::GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted) :
    clientStarted_(clientStarted)   ,
    clientAccepted_(clientAccepted) ,
    gameId_(gameId)                 ,
    state_(ST_PLACING)
{

}

GameController::~GameController()
{

}


ClientsIterator GameController::getClientStartedIt()
{
    return clientStarted_;
}

ClientsIterator GameController::getClientAcceptedIt()
{
    return clientAccepted_;
}

int GameController::getGameId()
{
    return gameId_;
}

GameController::GameState GameController::getState()
{
    return state_;
}

//void GameController::startGame()
//{

//}
