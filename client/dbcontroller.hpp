#ifndef DBCONTROLLER_HPP
#define DBCONTROLLER_HPP

#include <QDebug>
#include <QDialog>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlTableModel>

class DBController : public QDialog
{
    Q_OBJECT
public:
    explicit DBController(QWidget* parent = nullptr);
    ~DBController();

    void connectDatabase(const QString& dbName);
    void disconnectDatabase();
    void runQuery(QString queryStr);
    void createTable(QString tableName, QString tableFormat);

    void printTable(const QString& tableName);
    int tableLen(const QString& tableName);
    void clearDatabase();

    QString getRandomField();
    void addNewPlacement(QString field);

private:
    QSqlDatabase db_;
    QSqlQuery *query_;
    QSqlTableModel *model_;
};

#endif // DBCONTROLLER_HPP
