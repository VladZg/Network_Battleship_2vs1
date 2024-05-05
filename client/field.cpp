#include <QPixmap>
#include <QPainter>
#include "field.h"
#include "images.h"
#include <iostream>

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

void Field::setCell(int x, int y, CellDraw cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldDraw_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

QString Field::getFieldStr() const
{
    QString result = "";

    for(QVector<CellDraw>::const_iterator cell_it = fieldDraw_.begin(); cell_it != fieldDraw_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

void Field::setField(QString field)
{
    if (field.size() != area_)
        return;

    fieldDraw_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if ((*cell_it) < (QChar)CELL_EMPTY || (*cell_it) > (QChar)CELL_MARK)
            fieldDraw_.push_back((CellDraw)cell_it->digitValue());
    }
}

void Field::setField(QVector<CellDraw> field)
{
    if (field.size() != area_)
        return;

    fieldDraw_ = field;
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

int Field::shipNum(int size) const  // checks the number of ships with <size> blocks
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

bool Field::isShip(int size, int x, int y) const
{
    // TODO: implement function that checks if thats a ship with needed <size>

    return true;
}

bool Field::CheckDiagonalCollisions(QVector<CellState> fieldState) const
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

bool Field::CheckLength(QVector<CellState> fieldState) const
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

            if(fieldState[index + (width_ + 2)] != CL_ST_EMPTY && fieldState[index + 1] != CL_ST_EMPTY)
            {
               fieldState[index] = CI_ST_LEFT;
               //hotizontal ship
               index++;
               int length = 2;

               while(fieldState[index+1]!=CL_ST_EMPTY)
               {
                    fieldState[index] = CI_ST_HMIDDLE;
                    index++;
                    length++;
               }
               fieldState[index] = CI_ST_RIGHT;

               if(length > 4)
                   return false;

               continue;
            }

            if(fieldState[index + 1] == CL_ST_EMPTY && fieldState[index + (width_ + 2)] != CL_ST_EMPTY)
            {
               fieldState[index] = CI_ST_TOP;

               //vertical ship
               continue;
            }

            fieldState[index] = CI_ST_CENTER;

        }
    }

    return true;
}

bool Field::isCorrect() const
{
    QVector<CellState> fieldStateNoBorder(fieldState_);
    QVector<CellState> fieldState((width_ + 2) * (height_ + 2));

    std::copy(fieldStateNoBorder.begin(), fieldStateNoBorder.end(), fieldState.begin());

    fieldStateNoBorder[0] = CI_ST_CENTER;
    fieldStateNoBorder[1] = CI_ST_CENTER;
    fieldStateNoBorder[3] = CI_ST_CENTER;
    fieldStateNoBorder[4] = CI_ST_CENTER;
    fieldStateNoBorder[5] = CI_ST_CENTER;
    fieldStateNoBorder[6] = CI_ST_CENTER;
    fieldStateNoBorder[7] = CI_ST_CENTER;

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


    QDebug debug = qDebug();
    debug << "\n";
    for(int i = 0; i < height_ + 2; i++)
    {
        for(int j = 0; j < width_ + 2; j++)
        {
            debug << fieldState[i * 12 + j] << " ";
        }
        debug << "\n";
    }


        // Check field for correct ship placement
    //    return  (shipNum(1) == 4 &&
    //             shipNum(2) == 3 &&
    //             shipNum(3) == 2 &&
    //             shipNum(4) == 1   );

    return true;
}

QVector<CellState> Field::getFieldState()
{
    return fieldState_;
}
