#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "config.hpp"
#include "constants.hpp"

enum CellDraw       // состояния клетки для отрисовки
{
    CELL_EMPTY = 0  ,   // пустое поле
    CELL_LIVE       ,   // часть живого корабля (ещё не попали)
    CELL_DOT        ,   // выстрел - промах
    CELL_DAMAGED    ,   // подбитое поле частично подбитого корабля
    CELL_KILLED     ,   // поле полностью подбитого корабля
    CELL_MARK       ,   // помеченное пользователем поле (как флажок в сапёре)
};

enum CellState  // состояния клетки
{
    CL_ST_EMPTY = 0,    // пустая клетка
    CL_ST_CENTER   ,    // центральная клетка (единичный корабль)
    CL_ST_TOP      ,    // верхняя клетка корабля (вертикального)
    CL_ST_BOTTOM   ,    // нижняя клетка корабля (вертикального)
    CL_ST_VMIDDLE  ,    // серединная клетка вертикально ориентированного корабля
    CL_ST_HMIDDLE  ,    // серединная клетка горизонтально ориентированного корабля
    CL_ST_LEFT     ,    // левая клетка корабля (горизонтального)
    CL_ST_RIGHT    ,    // правая клетка корабля (горизонтального)

    CL_UNDEFINED   ,    // неопределённое непустое состояние клетки
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
    Field& operator=(const Field& other);   // copy assignment

    ~Field();

    CellDraw getCell(int x, int y) const;
    void setDrawCell(int x, int y, CellDraw cell);
    void setStateCell(int x, int y, CellState cell);
    QString getStateFieldStr() const;
    QString getDrawFieldStr() const;

    void setStateField(QString field);
    void setDrawField(QString field);
    void setDrawField(QVector<CellDraw> field);
    void setStateField(QVector<CellState> field);
    void initDrawField();

    void clear();
    QImage getFieldImage();

    void generate();

    int getWidth() const;
    int getHeight() const;

    bool isCorrect() const;
    bool CheckDiagonalCollisions(QVector<CellState> fieldState) const;
//    bool CheckVerticalCollisions(QVector<CellState> fieldState) const;
    bool CheckLength(QVector<CellState> fieldState) const;

private:
    int shipNum( int size ) const;
    bool isShip( int size, int x, int y ) const;

private:
    int width_;
    int height_;
    int area_;
    QVector<CellState> fieldState_;
    QVector<CellDraw> fieldDraw_;
};

#endif // FIELD_H
