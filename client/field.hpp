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
    CL_ST_EMPTY     = 0,    // пустая клетка
    CL_ST_CENTER    = 1,    // центральная клетка (единичный корабль)
    CL_ST_TOP       = 2,    // верхняя клетка корабля (вертикального)
    CL_ST_BOTTOM    = 3,    // нижняя клетка корабля (вертикального)
    CL_ST_VMIDDLE   = 4,    // серединная клетка вертикально ориентированного корабля
    CL_ST_HMIDDLE   = 5,    // серединная клетка горизонтально ориентированного корабля
    CL_ST_LEFT      = 6,    // левая клетка корабля (горизонтального)
    CL_ST_RIGHT     = 7,    // правая клетка корабля (горизонтального)
    CL_ST_UNDEFINED = 8,    // неопределённое непустое состояние клетки
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
    void setStateCell(int index_bordered, CellState cell);
    QString getStateFieldStr() const;
    QString getDrawFieldStr() const;
    QVector<CellDraw> getDrawField() const;

    void setStateField(QString field);
    void setDrawField(QString field);
    void setDrawField(QVector<CellDraw> field);
    void setStateField(QVector<CellState> field);
    void initMyDrawField();

    void clear();
    QImage getFieldImage();

    void generate();

    int getWidth() const;
    int getHeight() const;
    int getArea() const;

    bool isCorrect();
    bool CheckDiagonalCollisions(const QVector<CellState>& fieldStateWithBorders);
    bool CheckLength(QVector<CellState>& fieldState);

private:
    int shipNum(int size, const QVector<CellState>& fieldStateWithBorders);
    bool isShip(int size, int x, int y, const QVector<CellState>& fieldStateWithBorders);

private:
    int width_;
    int height_;
    int area_;
    QVector<CellState> fieldState_;
    QVector<CellDraw> fieldDraw_;
};

QVector<CellDraw> fieldDrawFromStr(QString fieldDrawStr);

#endif // FIELD_H
