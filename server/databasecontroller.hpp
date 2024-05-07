#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QDialog>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlTableModel>

class DBController:public QDialog
{
    Q_OBJECT
public:
    explicit DBController(QWidget *parent = nullptr);
    ~DBController();

private:
    QSqlDatabase db_;
    QSqlQuery *query_;
    QSqlTableModel *model_;
};

#endif // DATABASECONTROLLER_H
