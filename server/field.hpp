#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "./config.hpp"

enum Cell
{
    CELL_EMPTY = 0  ,   // пустое поле
    CELL_SHIP       ,   // есть корабль
};

class Field
{
public:
    Field();
    Field(QString field);
    Field(QString field, QString fieldState);
    ~Field();

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

    enum CellDraw       // состояния клетки для отрисовки
    {
        CELL_EMPTY = 0  ,   // пустое поле
        CELL_LIVE       ,   // часть живого корабля (ещё не попали)
        CELL_DOT        ,   // выстрел - промах
        CELL_DAMAGED    ,   // подбитое поле частично подбитого корабля
        CELL_KILLED     ,   // поле полностью подбитого корабля
        CELL_MARK       ,   // помеченное пользователем поле (как флажок в сапёре)
    };

    Cell getCell(int x, int y);
    QVector<Cell> getField();
    int getWidth();
    int getHeight();
    QString getFieldStr();
    QString getFieldStateStr();
    QString getFieldDrawStr();
    QVector<CellState> getFieldState();
    QVector<CellDraw> getFieldDraw();

    void setCell(int x, int y, Cell cell);
    void setField(QString field);
    void setCellState(int x, int y, CellState state);
    void setCellDraw(int x, int y, CellDraw state);
    void setFieldState(QString field);
    void setFieldDraw(QVector<Field::CellDraw> field);

    void clear();
    void generate();
    void initFieldDraw();
    void initFieldState();

    bool isCellEmpty(int x, int y);
    bool isCorrect() const;
    bool isKilled(int x, int y);

private:
    int width_;
    int height_;
    int area_;
    QVector<Cell> field_;
    QVector<CellState> fieldState_;
    QVector<CellDraw> fieldDraw_;
};

#endif // FIELD_H
