#include "gamecontroller.hpp"

GameController::GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted) :
    clientStarted_(clientStarted)   ,
    clientAccepted_(clientAccepted) ,
    gameId_(gameId)                 ,
    state_(ST_PLACING)              ,
    nPlaced_(0)
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

void GameController::incNPlaced()
{
    nPlaced_++;
}

int GameController::getNPlaced()
{
    return nPlaced_;
}

//void GameController::startGame()
//{

//}

void GameController::setClientStartedField(QString field)
{
    clientStartedField_.setField(field);
}

void GameController::setClientAcceptedField(QString field)
{
    clientAcceptedField_.setField(field);
}
