#ifndef MODEL_H
#define MODEL_H


#include <QString>
#include <QObject>
#include "config.h"
#include "constants.h"
#include "field.h"

enum ModelState
{
    ST_GAME_NSTARTED = 0,
    ST_PLACING_SHIPS    ,
    ST_WAITING_STEP     ,
    ST_MAKING_STEP      ,
    ST_GAME_FINISHED    ,
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
    CellDraw getEnemyCell(int x, int y) const;
    void setEnemyCell(int x, int y, CellDraw cell);
//    void setMyField(Field field);
    void setMyField(QVector<CellDraw> field);
    void setMyField(QString field);
    QString getMyFieldStr() const;
    Field getMyField() const;
    Field getEnemyField() const;
    bool checkMyField() const;
    void clearMyField();

    void startGame(QString enemy_login);
    void finishGame();

private:
    int shipNum( int size ) const;
    bool isShip( int size, int x, int y ) const;

private:
    Field* myField_;
    Field* enemyField_;
    ModelState state_;
    QString login_;
};


#endif // MODEL_H
