#include "field.h"

Field::Field() :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
}

Field::Field(QString field)
{

}

Field::~Field()
{
    field_.clear();
}

CellDraw Field::getCell(int x, int y) const
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        return field_[width_*y+x];
    }

    qDebug() << "Wrong cell indexes";
    return CELL_EMPTY;
}

void Field::setCell(int x, int y, CellDraw cell)
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

    for(QVector<CellDraw>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
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
            field_.push_back((CellDraw)cell_it->digitValue());
    }
}

void Field::setField(QVector<CellDraw> field)
{
    if (field.size() != area_)
        return;

    field_ = field;
}

void Field::clear()
{
    field_.fill(CELL_EMPTY, area_);
}

int Field::getWidth() const
{
    return width_;
}

int Field::getHeight() const
{
    return height_;
}


