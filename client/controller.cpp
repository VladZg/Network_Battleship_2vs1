#include "controller.hpp"
#include <QMouseEvent>

Controller::Controller(Model* model):
    model_(model)
{

}

Controller::~Controller()
{

}

QPoint getFieldCoord(const QPoint& pos, Field::Owner owner)
{
    QPoint res;
    res.setX(-1);
    res.setY(-1);

    int shift_x = 0;
    int shift_y = 0;

    if (owner == Field::MY_FIELD)
    {
        shift_x = MYFIELD_IMG_X;
        shift_y = MYFIELD_IMG_Y;
    }
    else
    {
        shift_x = ENEMYFIELD_IMG_X;
        shift_y = ENEMYFIELD_IMG_Y;
    }

    if(pos.x() < shift_x || pos.x() > (shift_x + FIELD_IMG_WIDTH_DEFAULT ) ||
       pos.y() < shift_y || pos.y() > (shift_y + FIELD_IMG_HEIGHT_DEFAULT)   )
        return res;

    res.setX(1.0*FIELD_WIDTH_DEFAULT *(pos.x()-shift_x)/FIELD_IMG_WIDTH_DEFAULT );
    res.setY(1.0*FIELD_HEIGHT_DEFAULT*(pos.y()-shift_y)/FIELD_IMG_HEIGHT_DEFAULT);

    qDebug() << "X: " << pos.x() - shift_x;
    qDebug() << "Y: " << pos.y() - shift_y;

    return res;
}

void Controller::onMousePressed(const QPoint& pos, QMouseEvent* event)
{
    if(model_->getState() == ST_PLACING_SHIPS)
    {
        QPoint point = getFieldCoord(pos, Field::MY_FIELD);

        if( point.x() == -1 || point.y() == -1 )
            return;

        qDebug() << "Ship at" << point.x() << point.y();

        if (event->button() == Qt::LeftButton)
        {
            model_->setMyDrawCell(point.x(), point.y(), CELL_LIVE);
            model_->setMyStateCell(point.x(), point.y(), CL_ST_UNDEFINED);
            qDebug() << "Press on left button -> place a ship";
        }
        if (event->button() == Qt::RightButton)
        {
            model_->setMyDrawCell(point.x(), point.y(), CELL_EMPTY);
            model_->setMyStateCell(point.x(), point.y(), CL_ST_EMPTY);
            qDebug() << "Press on left button -> remove a ship";
        }
        else
            qDebug() << "nothing to do on this button click";

//        emit stateChanged();

//        if (model_->getState() == ST_PLACING_SHIPS)
//        {
//            if (model_->isMyFieldCorrect())
//            {
//                isCorrectLabel->setText("корректная расстановка");
//            }
//            else

//            {
//                isCorrectLabel->setText("некорректная расстановка");
//            }
//        }

        qDebug() << "field after click: " << model_->getMyFieldStr();

        return;
    }

    if(model_->getState() == ST_MAKING_STEP)
    {
        QPoint point = getFieldCoord(pos, Field::ENEMY_FIELD);
        if(point.x() == -1 || point.y() == -1)
            return;

        if (event->button() == Qt::LeftButton)
        {
//             TODO: request to the server and habndle it
//            CellDraw status = CELL_DOT; // get from server
//            model_->setEnemyCell(point.x(), point.y(), status);
            qDebug() << "Press on left button -> make a shot";
        }
        else if (event->button() == Qt::RightButton)
        {
            if (model_->getEnemyCell(point.x(), point.y()) != CELL_EMPTY)
            {
                qDebug() << "This cell if already played, cannot mark it";
                return;
            }

            model_->setEnemyCell(point.x(), point.y(), CELL_MARK);
            qDebug() << "Press on right button -> place a mark";
        }
        else
            qDebug() << "nothing to do on this button click";

        qDebug() << "Going to" << point.x() << point.y();
//        CellDraw cell = model->getEnemyCell( point.x(), point.y() );

//        if(cell != CELL)
//            return;

//        model_->setEnemyCell( point.x(), point.y(), CL_DOT );

        // TODO: make step

//        emit stateChanged();
        return;
    }
}
