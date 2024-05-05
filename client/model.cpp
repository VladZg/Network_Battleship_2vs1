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
    // писать нахуй тут

    // Check field for correct ship placement
//    return  (shipNum(1) == 4 &&
//             shipNum(2) == 3 &&
//             shipNum(3) == 2 &&
//             shipNum(4) == 1   );

    return true;
}

int Model::shipNum(int size) const  // checks the number of ships with <size> blocks
{
    int shipNumber = 0;

    int width  = myField_->getWidth();
    int height = myField_->getHeight();

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(isShip(size, i, j))
                shipNumber++;
        }
    }

    return shipNumber;
}

bool Model::isShip(int size, int x, int y) const
{
    // TODO: implement function that checks if thats a ship with needed <size>

    return true;
}


void Model::startGame(QString enemy_login)
{
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
