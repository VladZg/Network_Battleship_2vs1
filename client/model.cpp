#include "./model.hpp"
#include <QMessageBox>

Model::Model() :
    state_(ST_GAME_NSTARTED)
{
    myField_    = new Field();
    enemyField_ = new Field();
}

Model::~Model()
{
    if (myField_)
        delete myField_;

    if (enemyField_)
        delete enemyField_;

    updateState(ST_GAME_FINISHED);
}

CellDraw Model::getMyCell(int x, int y) const
{
    return myField_->getCell(x, y);
}

CellDraw Model::getEnemyCell(int x, int y) const
{
    return enemyField_->getCell(x, y);
}

void Model::setMyDrawCell(int x, int y, CellDraw cell)
{
    myField_->setDrawCell(x, y, cell);
}

void Model::setMyStateCell(int x, int y, CellState cell)
{
    myField_->setStateCell(x, y, cell);
}

void Model::setEnemyCell(int x, int y, CellDraw cell)
{
    enemyField_->setDrawCell(x, y, cell);
}

QString Model::getMyFieldStr() const
{
    return myField_->getStateFieldStr();
}

Field Model::getMyField() const
{
    return *myField_;
}

Field Model::getEnemyField() const
{
    return *enemyField_;
}

void Model::setMyField(QVector<CellDraw> field)
{
    myField_->setDrawField(field);
}

void Model::setMyField(QVector<CellState> field)
{
    myField_->setStateField(field);
}

void Model::setMyField(QString field)
{
    myField_->setStateField(field);
}

void Model::initMyDrawField()
{
    myField_->initMyDrawField();
}

void Model::clearMyField()
{
    myField_->clear();
}

ModelState Model::getState() const
{
    return state_;
}

void Model::updateState(ModelState state)
{
    state_ = state;
}

void Model::switchStep()
{
    if (state_ == ST_MAKING_STEP)
        state_ = ST_WAITING_STEP;

    else if (state_ == ST_WAITING_STEP)
        state_ = ST_MAKING_STEP;
}

void Model::setLogin(const QString& login)
{
    login_ = login;
}

QString Model::getLogin() const
{
    return login_;
}

int Model::getGameId() const
{
//    if (state_ == ST_GAME_NSTARTED) return

    return gameId_;
}

bool Model::isMyFieldCorrect() const
{
    return myField_->isCorrect();;
}

void Model::startGame(QString enemy_login, int gameId)
{
    gameId_ = gameId;
    enemyLogin_ = enemy_login;
    // ToDO: ...

    updateState(ST_PLACING_SHIPS);
}

void Model::finishGame()
{
    myField_->clear();
    enemyField_->clear();

    // TODO:

    updateState(ST_GAME_FINISHED);
}

void Model::startFight()
{
    if (amIStarted_)
    {
        updateState(ST_MAKING_STEP);
        qDebug() << "1st step is mine";
    }

    else
    {
        updateState(ST_WAITING_STEP);
        qDebug() << "1st step isn't mine";
    }

    // TODO:
}

void Model::generateMyField()
{
    myField_->generate();
}

void Model::setStartedFlag(bool val)
{
    amIStarted_ = val;
}

bool Model::getStartedFlag()
{
    return amIStarted_;
}

void Model::setEnemyLogin(QString login)
{
    enemyLogin_ = login;
}

QString Model::getEnemyLogin()
{
    return enemyLogin_;
}
