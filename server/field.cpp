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
    clear();
    setField(field);
    initFieldState();
}

Field::Field(QString field, QString fieldState) :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
    setField(field);
    setFieldState(fieldState);
}

Cell Field::getCell(int x, int y)
{
    qDebug() << "Field::getCell(" << x << "," << y << ")";

    qDebug() << "bin  : " << getFieldStr();
    qDebug() << "state: " << getFieldStateStr();
    qDebug() << "draw : " << getFieldDrawStr();

    qDebug() << "width=" << width_ << " height=" << height_ << " sizes=" << field_.size() << "," << fieldState_.size() << "," << fieldDraw_.size();

    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
//        qDebug() << "LOL?";
        qDebug() << "HERE: " + QString::number(width_*y+x);
        return field_[width_*y+x];
//        return CELL_SHIP;
    }

//    qDebug() << "KEK?";

    qDebug() << "Wrong cell indexes";
    return Cell::CELL_EMPTY;
}

int Field::getWidth()
{
    return width_;
}

int Field::getHeight()
{
    return height_;
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

void printField(const QVector<Field::CellState>& field)
{
    int width = sqrt(field.size());
    int height = width;

    QDebug debugOut = qDebug();
    debugOut << "\n";
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           debugOut << (int)(field[i*width+j]) << " ";
        }
        debugOut << "\n";
    }
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

QString Field::getFieldStateStr()
{
    QString result = "";

    for(QVector<CellState>::iterator cell_it = fieldState_.begin(); cell_it != fieldState_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}


QString Field::getFieldDrawStr()
{
    QString result = "";

    for(QVector<CellDraw>::iterator cell_it = fieldDraw_.begin(); cell_it != fieldDraw_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

QVector<Field::CellState> Field::getFieldState()
{
    return fieldState_;
}

QVector<Field::CellDraw> Field::getFieldDraw()
{
    return fieldDraw_;
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

void Field::initFieldDraw()
{
    fieldDraw_.clear();

    for(QVector<Cell>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
    {
        if(*cell_it == Cell::CELL_EMPTY)
        {
            fieldDraw_.push_back(CellDraw::CELL_EMPTY);
        }
        else if(*cell_it == Cell::CELL_SHIP)
        {
            fieldDraw_.push_back(CellDraw::CELL_LIVE);
        }
    }
}

void Field::setFieldState(QString field)
{
    fieldState_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CL_ST_EMPTY || cell_it->digitValue() > (int)CL_ST_UNDEFINED)
        {
            qDebug() << "setFieldState(str): wrong string!";
            fieldState_.clear();
            return;
        }

        fieldState_.push_back((CellState)cell_it->digitValue());
    }
}

void Field::setFieldDraw(QVector<Field::CellDraw> field)
{
    fieldDraw_ = field;
}

void Field::clear()
{
    field_.fill(Cell::CELL_EMPTY, area_);
    fieldState_.fill(CellState::CL_ST_EMPTY, area_);
    fieldDraw_.fill(CellDraw::CELL_EMPTY), area_;
}

bool Field::isCellEmpty(int x, int y)
{
    qDebug() << "Field::isCellEmpty";
    return getCell(x, y) == Cell::CELL_EMPTY;
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

bool Field::isKilled(int x, int y)  // считаем, что в fieldState_ правильная расстановка
{
    qDebug() << "Checking if the ship is killed...!";

    QVector<CellState> fieldStateWithBorders((width_+2)*(height_+2));
    QVector<CellDraw> fieldDrawWithBorders((width_+2)*(height_+2));
    fieldStateWithBorders.fill(CL_ST_EMPTY);
    fieldDrawWithBorders.fill(CELL_EMPTY);

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldStateWithBorders[(width_+2)*(i+1)+(j+1)] = fieldState_[width_*i+j];
        }
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldDrawWithBorders[(width_+2)*(i+1)+(j+1)] = fieldDraw_[width_*i+j];
        }
    }

    printField(fieldStateWithBorders);

//    int damagedCellIndex = width_*y+x;
    setCellDraw(x, y, CellDraw::CELL_DAMAGED);

    int damagedCellIndexBordered = (width_+2)*(y+1)+(x+1);
    qDebug() << "fieldStateWithBorders[damagedCellIndex] (" << x << "," << y << ") => (" << damagedCellIndexBordered << ")" << fieldStateWithBorders[damagedCellIndexBordered];

    switch(fieldStateWithBorders[damagedCellIndexBordered])
    {
        case(CellState::CL_ST_CENTER):
        {
            return true;
        }

        case(CellState::CL_ST_TOP):
        {
            for(int curCell = damagedCellIndexBordered + (width_+2); fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell += (width_+2))
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CellState::CL_ST_LEFT):
        {
            for(int curCell = damagedCellIndexBordered + 1; fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; ++curCell)
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CellState::CL_ST_BOTTOM):
        {
            for(int curCell = damagedCellIndexBordered - (width_+2); fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell -= (width_+2))
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CellState::CL_ST_VMIDDLE):
        {
            for(int curCell = damagedCellIndexBordered + (width_+2); fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell += (width_+2))
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            for(int curCell = damagedCellIndexBordered - (width_+2); fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell -= (width_+2))
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CellState::CL_ST_HMIDDLE):
        {
            for(int curCell = damagedCellIndexBordered + 1; fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell += 1)
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            for(int curCell = damagedCellIndexBordered - 1; fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; curCell -= 1)
            {
                if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                    return false;
            }
            return true;
        }

    case(CellState::CL_ST_RIGHT):
    {
        for(int curCell = damagedCellIndexBordered - 1; fieldStateWithBorders[curCell] != CellState::CL_ST_EMPTY; --curCell)
        {
            if(fieldDrawWithBorders[curCell] != CellDraw::CELL_KILLED && fieldDrawWithBorders[curCell] != CellDraw::CELL_DAMAGED)
                return false;
        }
        return true;
    }

        default:
        {
            qDebug() << "From function isKilled: unknown cell state!";
            return false;
        }
    }

    return false;
}


void Field::setCellState(int x, int y, CellState state)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldState_[width_*y+x] = state;
        return;
    }
}

void Field::setCellDraw(int x, int y, CellDraw state)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldDraw_[width_*y+x] = state;
        return;
    }
}

void Field::initFieldState()
{
    QVector<CellState> fieldStateWithBorders((width_+2)*(height_+2));
    fieldStateWithBorders.fill(CL_ST_EMPTY);

    qDebug() << "void initFieldState(): <-heeeere0";

    fieldState_.clear();

    for(QVector<Cell>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
    {
        if (*cell_it == Cell::CELL_EMPTY)
            fieldState_.push_back(CellState::CL_ST_EMPTY);

        else if (*cell_it == Cell::CELL_SHIP)
            fieldState_.push_back(CellState::CL_ST_UNDEFINED);
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldStateWithBorders[(width_+2)*(i+1)+(j+1)] = fieldState_[width_*i+j];
//            qDebug() << "(" << (width_+2)*(i+1)+(j+1) << ") = " << fieldState_[width_*i+j];
//            qDebug() << "(" << i << ", " << j << ") = " << fieldState_[width_*i+j];
        }
    }

    qDebug() << "void initFieldState(): <-heeeere1";


    if (fieldStateWithBorders.size() != (height_+2)*(width_+2))
    {
        qDebug() << "Wrong size of the fieldStateWithBorders :" << fieldStateWithBorders.size() << " instead of " << (height_+2)*(width_+2);
        return;
    }

    qDebug() << "void initFieldState(): <-heeeere2";

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            int index = (width_+2)*(i + 1) + (j+1);
            if(fieldStateWithBorders[index] == CL_ST_UNDEFINED)
            {
//                qDebug() << "HERE1!";

                if(fieldStateWithBorders[index+1] != CL_ST_EMPTY) //horizontal ship (go from left toright)
                {
//                    qDebug() << "HERE2!";

                    fieldStateWithBorders[index] = CL_ST_LEFT;

                    int length = 2;
                    int delta = 1;
                    index += delta;

                    while(fieldStateWithBorders[index+delta] != CL_ST_EMPTY)
                    {
                         fieldStateWithBorders[index] = CL_ST_HMIDDLE;
                         index += delta;
                         length++;
                    }

                    fieldStateWithBorders[index] = CL_ST_RIGHT;

                    qDebug() << "length of the horizontal ship: " << length;

                    if(length > 4)
                        return;

                    continue;
                }

                if(fieldStateWithBorders[index+width_+2] != CL_ST_EMPTY) // vertical ship (go from up to down)
                {
                    fieldStateWithBorders[index] = CL_ST_TOP;

                    int length = 2;
                    int delta = width_+2;
                    index += delta;

                    while(fieldStateWithBorders[index+delta] != CL_ST_EMPTY)
                    {
                         fieldStateWithBorders[index] = CL_ST_VMIDDLE;
                         index += delta;
                         length++;
                    }

                    fieldStateWithBorders[index] = CL_ST_BOTTOM;

                    qDebug() << "length of the vertical ship: " << length;

                    if(length > 4)
                        return;

                    continue;
                 }

                 fieldStateWithBorders[index] = CL_ST_CENTER;
                 qDebug() << "length of the ship: 1";
            }
        }
    }

    qDebug() << "void initFieldState(): <-heeeere3";

//    printField(fieldStateWithBorders);

    for (int i = 0; i < height_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            fieldState_[width_*i+j] = fieldStateWithBorders[(width_+2)*(i+1)+(1+j)];
        }
    }

    qDebug() << "inited fieldState_:" ;
    printField(fieldState_);
}

QVector<Cell> Field::getField()
{
    return field_;
}
