#include <QPixmap>
#include <QPainter>
#include "field.h"
#include "images.h"

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

//            case CELL_PART:
//                painter.drawImage(x, y, pictures.get("part"));
//                break;

//            case CELL_KILL:
//                painter.drawImage(x, y, pictures.get("kill"));
//                break;

            case CELL_MARK:
            {
                painter.drawImage(x, y, pictures.get("mark"));
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
