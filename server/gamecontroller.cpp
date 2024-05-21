#include "gamecontroller.hpp"

GameController::GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted) :
    clientStarted_(clientStarted)                       ,
    clientAccepted_(clientAccepted)                     ,
    // clientStartedField_(clientStarted->getField())      ,
    // clientAcceptedField_(clientAccepted_->getField())   ,
    gameId_(gameId)                                     ,
    state_(ST_NSTARTED)                                 ,
    nPlaced_(0)                                         ,
    nDecks_(4*1+3*2+2*3+1*4)                            ,
    nStartedDamaged_(0)                                 ,
    nAcceptedDamaged_(0)                                ,
    winnerLogin_()
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

bool GameController::checkGameFinish(bool isStartedKilled)
{
    if (isStartedKilled)
        return nStartedDamaged_ == nDecks_;
    else
        return nAcceptedDamaged_ == nDecks_;
}

void GameController::updateState(GameController::GameState state)
{
    state_ = state;
    qDebug() << "game" << gameId_ << " state updated to " << state_;
}

GameController::GameState GameController::getState()
{
    return state_;
}

void GameController::incNPlaced()
{
    nPlaced_++;
}

void GameController::incNDamaged(bool isStartedDamaged)
{
    if (isStartedDamaged)
    {
        nStartedDamaged_++;
        qDebug() << "Increased nStartedDamaged_: " << nStartedDamaged_;
    }
    else
    {
        nAcceptedDamaged_++;
        qDebug() << "Increased nAcceptedDamaged_: " << nAcceptedDamaged_;
    }
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
