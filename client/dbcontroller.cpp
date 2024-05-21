#include "dbcontroller.hpp"
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <cstdlib>
#include <ctime>

DBController::DBController(QWidget* parent) : QDialog(parent)
{
    srand(time(nullptr));
}

DBController::~DBController()
{
}

void DBController::connectDatabase(const QString& dbName)
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbName);

    if(!db_.open())
    {
        qDebug() << "Error opening database: " << db_.lastError().text();
        return;
    }

    query_ = new QSqlQuery(db_);
}

void DBController::runQuery(QString queryStr)
{
    query_->exec(queryStr);
}

void DBController::createTable(QString tableName, QString tableFormat)
{
    if (!query_->exec("SELECT * FROM " + tableName))
    {
        // Если таблица не существует, создаем ее
        if (!query_->exec("CREATE TABLE " + tableName + "(" + tableFormat + ");"))
        {
            qDebug() << "Ошибка при создании таблицы " + tableName + ":" << query_->lastError().text();
            return;
        }
    }

    qDebug() << "Таблица " + tableName + " успешно создана или уже существует.";

    model_ = new QSqlTableModel(this, db_);
    model_->setTable(tableName);
    model_->select();

//    parent->ui->tableView->setModel(model_);
}

QString DBController::getRandomField()
{
    int nFields = tableLen("Fields");
    qDebug() << "nFields = " + QString::number(nFields);

    if (nFields <= 0)
    {
        qDebug() << "Нет расстановок в БД таблице Fields";
        return "";
    }

    int randomIndex = rand() % (nFields);

    // Запрос на получение случайной записи из таблицы
    query_->exec(QString("SELECT field_text FROM Fields LIMIT 1 OFFSET %1").arg(randomIndex));

    if (!query_->next())
    {
        qDebug() << "No random string found.";
        return "";
    }

    QString randomString = query_->value(0).toString();
    qDebug() << "Random field: " << randomString;
    return randomString;
}

void DBController::addNewPlacement(QString field)
{
    query_->prepare("INSERT INTO Fields (field_text) VALUES (:text)");
    query_->bindValue(":text", field);
    query_->exec();
}

void DBController::disconnectDatabase()
{
    if(!db_.isOpen())
    {
        qDebug() << "Wasn't opened";
        return;
    }

    db_.close();

    delete query_;
}

void DBController::clearDatabase()
{
    // Получаем список таблиц в базе данных
    QStringList tables = db_.tables();

    // Удаляем все данные из каждой таблицы
    foreach (const QString &table, tables)
    {
        if (!query_->exec("DELETE FROM " + table))
        {
            qDebug() << "Ошибка при удалении данных из таблицы" << table << ":" << query_->lastError().text();
        }
        else
        {
            qDebug() << "Данные успешно удалены из таблицы" << table;
        }
    }
}

void DBController::printTable(const QString& tableName)
{
    *query_ = QSqlQuery("SELECT * FROM " + tableName, db_);

    if (!query_->exec()) {
        qDebug() << "Ошибка при выполнении запроса:" << query_->lastError().text();
        return;
    }

    QSqlRecord rec = query_->record();
    int columns = rec.count();
    int lines = 0;

    for (;query_->next();lines++)
    {
        for (int i = 0; i < columns; ++i)
        {
            qDebug() << rec.fieldName(i) << ":" << query_->value(i).toString();
        }
        qDebug() << "-----------------------";
    }

    qDebug() << "  Имя таблицы: " << tableName;
    qDebug() << "Длина таблицы: " << lines;
}

int DBController::tableLen(const QString& tableName)
{
    *query_ = QSqlQuery("SELECT * FROM " + tableName, db_);

    if (!query_->exec()) {
        qDebug() << "Ошибка при выполнении запроса:" << query_->lastError().text();
        return 0;
    }

    QSqlRecord rec = query_->record();
    int columns = rec.count();
    int lines = 0;

    for (;query_->next();lines++)
        for (int i = 0; i < columns; ++i);

    return lines;
}
