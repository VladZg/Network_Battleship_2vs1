#include "./model.h"

Model::Model()
{
    myField_ = new Field();
//    state_ = ST_PLACING_SHIPS;
}

Model::~Model()
{
    if (myField_)
        delete myField_;
}

Cell Model::getMyCell(int x, int y) const
{
    return myField_->getCell(x, y);
}

void Model::setMyCell(int x, int y, Cell cell)
{
    myField_->setCell(x, y, cell);
}

QString Model::getMyFieldStr() const
{
    return myField_->getFieldStr();
}

Field Model::getMyField() const
{
    return *myField_;
}

void Model::setMyField(QVector<Cell> field)
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

void Model::setState(ModelState state)
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

bool Model::checkMyField() const
{
    // Check field for correct ship placement
    return (shipNum(1) == 4 &&
            shipNum(2) == 3 &&
            shipNum(3) == 2 &&
            shipNum(4) == 1   );
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
