#ifndef MODEL_H
#define MODEL_H


#include <QString>
#include <QObject>
#include "config.hpp"
#include "constants.hpp"
#include "field.hpp"

enum ModelState
{
    ST_GAME_NSTARTED = 0,
    ST_PLACING_SHIPS    ,
    ST_WAITING_PLACING  ,
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
    int getGameId() const;

    CellDraw getMyCell(int x, int y) const;
    void setMyDrawCell(int x, int y, CellDraw cell);
    void setMyStateCell(int x, int y, CellState cell);
    CellDraw getEnemyCell(int x, int y) const;
    void setEnemyCell(int x, int y, CellDraw cell);
//    void setMyField(Field field);
    void setMyField(QVector<CellDraw>  field);
    void setMyField(QVector<CellState> field);
    void setMyField(QString field);
    QString getMyFieldStr() const;
    Field getMyField() const;
    Field getEnemyField() const;
    bool isMyFieldCorrect() const;
    void clearMyField();

    void generateMyField();

    void startGame(QString enemy_login, int gameId);
    void finishGame();

private:
    Field* myField_;
    Field* enemyField_;
    ModelState state_;
    QString login_;
    int gameId_;
};


#endif // MODEL_H