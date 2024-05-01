#include "field.h"

Field::Field() :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
}

Field::~Field()
{

}

Field::Field(QString field)
{
    setField(field);
}

Cell Field::getCell(int x, int y)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        return field_[width_*y+x];
    }

    qDebug() << "Wrong cell indexes";
    return CELL_EMPTY;
}

void Field::setCell(int x, int y, Cell cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        field_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

QString Field::getFieldStr()
{
    QString result = "";

    for(QVector<Cell>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

void Field::setField(QString field)
{
    if (field.size() != area_)
        return;

    field_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if ((*cell_it) < (QChar)CELL_EMPTY || (*cell_it) > (QChar)CELL_MARK)
        field.push_back(QString(*cell_it));
    }
}

void Field::setField(QVector<Cell> field)
{
    if (field.size() != area_)
        return;

    field_ = field;
}

void Field::clear()
{
    field_.fill(CELL_EMPTY, area_);
}

bool Field::isCorrect() const  // check if ship placement is correct
{
    // TODO: check for correct or not

    return true;
}
