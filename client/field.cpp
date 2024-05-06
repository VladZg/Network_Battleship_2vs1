#include <QPixmap>
#include <QPainter>
#include "field.hpp"
#include "images.hpp"

Field::Field() :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
}

Field::Field(QString field) :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    qDebug() << "Field(str) constructor: " << field;
    setStateField(field);
}

Field& Field::operator=(const Field& other)
{
    if (this == &other)
        return *this;

    width_      = other.width_      ;
    height_     = other.height_     ;
    area_       = other.area_       ;
    fieldState_ = other.fieldState_ ;
    fieldDraw_  = other.fieldDraw_  ;

    return *this;
}

Field::~Field()
{
    fieldState_.clear();
    fieldDraw_.clear();
}

CellDraw Field::getCell(int x, int y) const
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        return fieldDraw_[width_*y+x];
    }

    qDebug() << "Wrong cell indexes";
    return CELL_EMPTY;
}

void Field::setDrawCell(int x, int y, CellDraw cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldDraw_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

void Field::setStateCell(int x, int y, CellState cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
//        qDebug() << (int)cell;
        fieldState_[width_*y+x] = cell;
//        qDebug() << fieldState_[width_*y+x];
//        qDebug() << "field after set: " << getStateFieldStr();
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

void Field::setStateCell(int index_bordered, CellState cell)
{
    if(index_bordered < (width_ + 2) * (height_ + 2))
    {
        fieldState_[index_bordered - 13 - 2*int(index_bordered/23)] = cell;
        return;
    }
}

QString Field::getStateFieldStr() const
{
    QString result = "";

    for(QVector<CellState>::const_iterator cell_it = fieldState_.begin(); cell_it != fieldState_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

QString Field::getDrawFieldStr() const
{
    QString result = "";

    for(QVector<CellDraw>::const_iterator cell_it = fieldDraw_.begin(); cell_it != fieldDraw_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

void Field::setStateField(QString field)
{
    fieldState_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CL_ST_EMPTY || cell_it->digitValue() > (int)CL_UNDEFINED)
        {
            qDebug() << "setStateField(str): wrong string!";
            fieldState_.clear();
            return;
        }

        fieldState_.push_back((CellState)cell_it->digitValue());
    }
}

void Field::setDrawField(QString field)
{
    if (field.size() != area_)
        return;

    fieldDraw_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CELL_EMPTY || cell_it->digitValue() > (int)CELL_MARK)
        {
            qDebug() << "setStateField(str): wrong string!";
            fieldDraw_.clear();
            return;
        }

        fieldDraw_.push_back((CellDraw)cell_it->digitValue());
    }
}

void Field::setStateField(QVector<CellState> field)
{
    if (field.size() != area_)
        return;

    fieldState_ = field;
}

void Field::setDrawField(QVector<CellDraw> field)
{
    if (field.size() != area_)
        return;

    fieldDraw_ = field;
}

void Field::initDrawField()
{
    fieldDraw_.clear();

    for(int i = 0; i < area_; i++)
    {
        if (fieldState_[i] == CL_ST_EMPTY)
            fieldDraw_.push_back(CELL_EMPTY);

        else
            fieldDraw_.push_back(CELL_LIVE);

//        qDebug() << QString::number(i) + ": " << fieldDraw_[i];
    }
}

void Field::clear()
{
    fieldDraw_.fill(CELL_EMPTY, area_);
    fieldState_.fill(CL_ST_EMPTY, area_);
}

int Field::getWidth() const
{
    return width_;
}

int Field::getHeight() const
{
    return height_;
}

QImage Field::getFieldImage()
{
    QImage image(FIELD_IMG_WIDTH_DEFAULT, FIELD_IMG_HEIGHT_DEFAULT, QImage::Format_ARGB32);
    CellDraw cell;
    image.fill(0);  // empty image
    QPainter painter(&image);

//    qDebug() << getFieldStr();

    double cfx = 1.0 * FIELD_IMG_WIDTH_DEFAULT /FIELD_WIDTH_DEFAULT ;
    double cfy = 1.0 * FIELD_IMG_HEIGHT_DEFAULT/FIELD_HEIGHT_DEFAULT;

    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < height_; j++)
        {
            cell = getCell(i, j);

            int x = i*cfx;
            int y = j*cfy;

            switch(cell)
            {
            case CELL_DOT:
            {
                painter.drawImage(x, y, pictures.get("dot"));
//                qDebug() << "dot";
                break;
            }

            case CELL_LIVE:
            {
                painter.drawImage(x, y, pictures.get("live"));
//                qDebug() << "live";
                break;
            }

            case CELL_DAMAGED:
            {
                painter.drawImage(x, y, pictures.get("damaged"));
                //                qDebug() << "damaged";
                break;
            }

            case CELL_KILLED:
            {
                painter.drawImage(x, y, pictures.get("killed"));
                //                qDebug() << "killed";
                break;
            }

            case CELL_MARK:
            {
                painter.drawImage(x, y+1, pictures.get("flag"));
//                qDebug() << "mark";
                break;
            }

            default:
                break;
            }
        }
    }

    painter.end();

    return image;
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

    setStateField(field_example);
    initDrawField();

    qDebug() << "Generated field (state): " + getStateFieldStr();
    qDebug() << "Generated field (draw ): " + getDrawFieldStr();

}

int Field::shipNum(int size)  // checks the number of ships with <size> blocks
{
    int shipNumber = 0;

    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < height_; j++)
        {
            if(isShip(size, i, j))
                shipNumber++;
        }
    }

    return shipNumber;
}

bool Field::isShip(int size, int x, int y)
{
    // TODO: implement function that checks if thats a ship with needed <size>

    return true;
}

bool Field::CheckDiagonalCollisions(QVector<CellState> fieldState)
{
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            int index = (width_ + 2)*(i + 1) + j + 1;
            if(fieldState[index] == CL_ST_EMPTY)
            {
                continue;
            }

            if(fieldState[index - (width_ + 2) - 1] != CL_ST_EMPTY ||
               fieldState[index - (width_ + 2) + 1] != CL_ST_EMPTY ||
               fieldState[index + (width_ + 2) + 1] != CL_ST_EMPTY ||
               fieldState[index + (width_ + 2) - 1] != CL_ST_EMPTY)
            {
                return false;
            }
        }
    }

    return true;
}

bool Field::CheckLength(QVector<CellState> fieldState) // тут если чё нихуя правильно не работает, надеюсь бы отдебажим как проснемся :))
{
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            int index = (width_ + 2)*(i + 1) + j + 1;
            if(fieldState[index] == CL_UNDEFINED)
            {
                if(fieldState[index + 1] != CL_ST_EMPTY) //horizontal ship
                 {
//                    setStateCell(index, CL_ST_LEFT);
//                    fieldState[index] = CL_ST_LEFT;

//                    int length = 2;
//                    int delta = 1;
//                    index += delta;

//                    while(fieldState[index + delta] != CL_ST_EMPTY)
//                    {
//                         setStateCell(index, CL_ST_HMIDDLE);
//                         fieldState[index] = CL_ST_HMIDDLE;
//                         index += delta;
//                         length++;
//                    }
//                    setStateCell(index, CL_ST_RIGHT);
//                    fieldState[index] = CL_ST_RIGHT;

//                    if(length > 4)
//                        return false;

//                    continue;
                 }

                 if(fieldState[index + (width_ + 2)] != CL_ST_EMPTY) //vertical ship
                 {
                     qDebug() << "                                                                          ЗАШЛИ В ИФ";
                    setStateCell(index, CL_ST_TOP);
                    fieldState[index] = CL_ST_TOP;

                    int step = 1;
                    int delta = width_ + 2;

                    while(fieldState[index + delta*step] != CL_ST_EMPTY)
                    {
                         setStateCell(index + delta*step, CL_ST_VMIDDLE);
                         fieldState[index + delta*step] = CL_ST_VMIDDLE;
                         step++;
                    }
                    setStateCell(index + delta*(step-1), CL_ST_BOTTOM);
                    fieldState[index + delta*(step-1)] = CL_ST_BOTTOM;

                    if(step > 4)
                        return false;

                    continue;
                 }

                 setStateCell(index, CL_ST_CENTER);
                 fieldState[index] = CL_ST_CENTER;
            }
        }
    }

    QDebug debug1 = qDebug();
    debug1 << "\n";
    for(int i = 0; i < height_ + 2; i++)
    {
        for(int j = 0; j < width_ + 2; j++)
        {
            debug1 << fieldState[i * 12 + j] << " ";
        }
        debug1 << "\n";
    }
    return true;
}

bool Field::isCorrect()
{
    QVector<CellState> fieldStateNoBorder(fieldState_);
    QVector<CellState> fieldState((width_ + 2) * (height_ + 2));

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldState[(width_ + 2)*(i + 1) + j + 1] = fieldStateNoBorder[width_*i + j];
        }
    }

    if(CheckDiagonalCollisions(fieldState) == false)
    {
        return false;
    }

    if(CheckLength(fieldState) == false)
    {
        return false;
    }

//    QDebug debug1 = qDebug();
//    debug1 << "\n";
//    for(int i = 0; i < height_ + 2; i++)
//    {
//        for(int j = 0; j < width_ + 2; j++)
//        {
//            debug1 << fieldState[i * 12 + j] << " ";
//        }
//        debug1 << "\n";
//    }

//    QDebug debug2 = qDebug();
//    debug2 << "\n";
//    for(int i = 0; i < height_; i++)
//    {
//        for(int j = 0; j < width_; j++)
//        {
//            debug2 << fieldState_[i * 10 + j] << " ";
//        }
//        debug2 << "\n";
//    }




        // Check field for correct ship placement
    //    return  (shipNum(1) == 4 &&
    //             shipNum(2) == 3 &&
    //             shipNum(3) == 2 &&
    //             shipNum(4) == 1   );

    return true;
    // random string
}
