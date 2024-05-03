#ifndef MODEL_H
#define MODEL_H


#include <QString>
#include <QObject>
#include "field.h"

enum ModelState
{
    ST_GAME_NSTARTED = 0,
    ST_PLACING_SHIPS    ,
    ST_WAITING_STEP     ,
    ST_MAKING_STEP      ,
};

class Model: public QObject
{
    Q_OBJECT

public:
    Model();
    ~Model();

    ModelState getState() const;
    void updateState(ModelState);
    void setLogin(const QString& str);
    QString getLogin() const;

    CellDraw getMyCell(int x, int y) const;
    void setMyCell(int x, int y, CellDraw cell);
//    void setMyField(Field field);
    void setMyField(QVector<CellDraw> field);
    void setMyField(QString field);
    QString getMyFieldStr() const;
    Field getMyField() const;
    bool checkMyField() const;
    void clearMyField();

private:
    int shipNum( int size ) const;
    bool isShip( int size, int x, int y ) const;

private:
    Field* myField_;
    ModelState state_;
    QString login_;
};


#endif // MODEL_H
