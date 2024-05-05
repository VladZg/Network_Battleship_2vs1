#include "./model.h"
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

void Model::setMyCell(int x, int y, CellDraw cell)
{
    myField_->setCell(x, y, cell);
}

void Model::setEnemyCell(int x, int y, CellDraw cell)
{
    enemyField_->setCell(x, y, cell);
}

QString Model::getMyFieldStr() const
{
    return myField_->getFieldStr();
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
    myField_->setField(field);
}

void Model::setMyField(QString field)
{
    myField_->setField(field);
}

ModelState Model::getState() const
{
    return state_;
}

void Model::updateState(ModelState state)
{
    state_ = state;
}

void Model::setLogin(const QString& login)
{
    login_ = login;
}

QString Model::getLogin() const
{
    return login_;
}

bool Model::isMyFieldCorrect() const
{
    Field MyField;

    return MyField.isCorrect();
}

void Model::startGame(QString enemy_login)
{
    // ToDO: ...
}

void Model::finishGame()
{
    myField_->clear();
    enemyField_->clear();

    // TODO:
}
