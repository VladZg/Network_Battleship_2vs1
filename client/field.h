#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "./config.h"
#include "constants.h"

enum CellDraw       // состояния клетки для отрисовки
{
    CELL_EMPTY = 0  ,   // пустое поле
    CELL_LIVE       ,   // часть живого корабля (ещё не попали)
    CELL_DOT        ,   // выстрел - промах
    CELL_DAMAGED    ,   // поле частично подбитого корабля
    CELL_KILLED     ,   // поле полностью подбитого корабля
    CELL_MARK       ,   // помеченное пользователем поле (как флажок в сапёре)
};

enum CellState  // состояния клетки
{
    CL_ST_EMPTY = 0,    // пустая клетка
    CI_ST_CENTER   ,    // центральная клетка (единичный корабль)
    CI_ST_TOP      ,    // верхняя клетка корабля (вертикального)
    CI_ST_BOTTOM   ,    // нижняя клетка корабля (вертикального)
    CI_ST_VMIDDLE  ,    // серединная клетка вертикально ориентированного корабля
    CI_ST_HMIDDLE  ,    // серединная клетка горизонтально ориентированного корабля
    CI_ST_LEFT     ,    // левая клетка корабля (горизонтального)
    CI_ST_RIGHT    ,    // правая клетка корабля (горизонтального)
};

class Field
{
public:
    enum Owner
    {
        MY_FIELD    = 0,
        ENEMY_FIELD = 1,
    };

public:
    Field();
    Field(QString field);
    ~Field();

    CellDraw getCell(int x, int y) const;
    void setCell(int x, int y, CellDraw cell);
    QString getFieldStr() const;
    void setField(QString field);
    void setField(QVector<CellDraw> field);
    void clear();
    QImage getFieldImage();

    int getWidth() const;
    int getHeight() const;

private:
    int width_;
    int height_;
    int area_;
    QVector<CellState> fieldState_;
    QVector<CellDraw> fieldDraw_;
};

#endif // FIELD_H
