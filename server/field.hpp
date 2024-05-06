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
    ~Field();

    Cell getCell(int x, int y);
    void setCell(int x, int y, Cell cell);
    QString getFieldStr();
    void setField(QString field);
//    void setField(QVector<Cell> field);
    void clear();

    bool isCorrect() const;
    void generate();

private:
    int width_;
    int height_;
    int area_;
    QVector<Cell> field_;
};

#endif // FIELD_H
