#include "controller.hpp"
#include <QMouseEvent>

Controller::Controller(Model* model, QTcpSocket* socket):
    model_(model),
    socket_(socket)
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
    ModelState state = model_->getState();

    if(state == ST_PLACING_SHIPS ||
       state == ST_GAME_NSTARTED ||
       state == ST_WAITING_PLACING )
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
            CellDraw cell = model_->getEnemyCell(point.x(), point.y());
            if (cell != CELL_EMPTY && cell != CELL_MARK)
            {
                qDebug() << "Already shot";
                return;
            }

            QString shotMessage = "GAME:" + QString::number(model_->getGameId()) + ":" + model_->getLogin() + ":" + "SHOT:" + QString::number(point.x()) + ":" + QString::number(point.y()); // GAME:<gameId>:<my_login>:SHOT:<x>:<y>

            socket_->write(shotMessage.toUtf8());
            qDebug() << shotMessage;
        }
        else if (event->button() == Qt::RightButton)
        {
            CellDraw cellSt = model_->getEnemyCell(point.x(), point.y());

            if (cellSt == CELL_MARK)
            {
                model_->setEnemyCell(point.x(), point.y(), CELL_EMPTY);
                qDebug() << "Press on right button -> place a mark";
                return;
            }

            if (cellSt != CELL_EMPTY)
            {
                qDebug() << "This cell if already played, cannot mark it";
                return;
            }

            model_->setEnemyCell(point.x(), point.y(), CELL_MARK);
            return;
        }
        else
        {
            qDebug() << "nothing to do on this button click";
            return;
        }

        qDebug() << "Going to" << point.x() << point.y();

        // TODO: make step

//        emit stateChanged();
        return;
    }

    if(model_->getState() == ST_WAITING_STEP)
    {
        QPoint point = getFieldCoord(pos, Field::ENEMY_FIELD);
        if(point.x() == -1 || point.y() == -1)
            return;

        if (event->button() == Qt::RightButton)
        {
            CellDraw cellSt = model_->getEnemyCell(point.x(), point.y());

            if (cellSt == CELL_MARK)
            {
                model_->setEnemyCell(point.x(), point.y(), CELL_EMPTY);
                qDebug() << "Press on right button -> place a mark";
                return;
            }

            if (cellSt != CELL_EMPTY)
            {
                qDebug() << "This cell if already played, cannot mark it";
                return;
            }

            model_->setEnemyCell(point.x(), point.y(), CELL_MARK);
            return;
        }
        else
        {
            qDebug() << "nothing to do on this button click";
            return;
        }
    }

}
