#include "gamecontroller.hpp"

GameController::GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted) :
    clientStarted_(clientStarted)                       ,
    clientAccepted_(clientAccepted)                     ,
    // clientStartedField_(clientStarted->getField())      ,
    // clientAcceptedField_(clientAccepted_->getField())   ,
    gameId_(gameId)                                     ,
    state_(ST_PLACING)                                  ,
    nPlaced_(0)                                         ,
    nDecks_(4*1+3*3+2*3+1*4)                            ,
    nDamaged_(0)
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

bool GameController::checkGameFinish()
{
    return nDamaged_ == nDecks_;
}

GameController::GameState GameController::getState()
{
    return state_;
}

void GameController::incNPlaced()
{
    nPlaced_++;
}

void GameController::incNDamaged()
{
    nDamaged_++;
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

void GameController::setClientStartedFieldState(QString field)
{
    clientStartedField_.setFieldState(field);
}

void GameController::setClientAcceptedFieldState(QString field)
{
    clientAcceptedField_.setFieldState(field);
}
