#include "field.hpp"

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

Field::Field(QString field) :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
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
    field_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CELL_EMPTY || cell_it->digitValue() > (int)CELL_SHIP)
        {
            qDebug() << "setField(str): wrong string!";
            field_.clear();
            return;
        }

        field_.push_back((Cell)cell_it->digitValue());
    }
}

//void Field::setField(QVector<Cell> field)
//{
//    if (field.size() != area_)
//        return;

//    field_ = field;
//}

void Field::clear()
{
    field_.fill(CELL_EMPTY, area_);
}

bool Field::isCorrect() const  // check if ship placement is correct
{
    // TODO: check for correct or not

    return true;
}

void Field::generate()
{
    qDebug() << "\"generate\" clicked: Generating new field";

    // TODO: add generated fields and atabase

    QString field_example = "1111011100"\
                            "0000000000"\
                            "1110110110"\
                            "0000000000"\
                            "1101010101"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000";   // example for testing

    setField(field_example);

    qDebug() << "Generated field (state): " + getFieldStr();
}
