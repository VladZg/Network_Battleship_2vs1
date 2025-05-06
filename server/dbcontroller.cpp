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

QStringList DBController::getGamesEndings()
{
    QSqlQuery query("SELECT * FROM GamesEndings");
    QStringList list;

    if (query.exec())
    {
        while (query.next())
        {
            const char* player1 = query.value(0).toString().toUtf8().constData();
            const char* player2 = query.value(1).toString().toUtf8().constData();
            const char* field_text1 = query.value(2).toString().toUtf8().constData();
            const char* field_text2 = query.value(3).toString().toUtf8().constData();
            const char* start_date = query.value(4).toString().toUtf8().constData();
            const char* end_date = query.value(5).toString().toUtf8().constData();
            const char* winner = query.value(6).toString().toUtf8().constData();

            QString formattedString = QString::asprintf("%s:%s:%s:%s:%s:%s:%s", player1, player2, field_text1, field_text2, start_date, end_date, winner);
            list.push_back(formattedString);
        }
    }

    return list;
}

void DBController::addNewPlacement(QString field)
{
    query_->prepare("INSERT INTO Fields (field_text) VALUES (:text)");
    query_->bindValue(":text", field);
    query_->exec();
}

void DBController::addNewGameEnding(GamesIterator gameIt)
{
    // Подготавливаем запрос для вставки новой записи в таблицу GamesEndings
    query_->prepare("INSERT INTO GamesEndings (player1, player2, field_text1, field_text2, start_date, end_date, winner)"
                    "VALUES (:player1, :player2, :field_text1, :field_text2, :start_date, :end_date, :winner)");

    // Конвертация карты из строки в формат заполненных и пустых квадратов поля 10x10
    QString firstPlayerField  = gameIt->getClientStartedIt()->getFieldStr();
    QString secondPlayerField = gameIt->getClientAcceptedIt()->getFieldStr();
    int firstFieldSize  = firstPlayerField.size();
    int secondFieldSize = secondPlayerField.size();

    QString myString = QString("■").repeated(10);
    QString formatFirstPlayerField  = QString("0").repeated(firstFieldSize);
    QString formatSecondPlayerField = QString("0").repeated(secondFieldSize);

    for(int i = 0; i < firstFieldSize; i++)
    {
        if(firstPlayerField[i] == '0')
        {
            formatFirstPlayerField[i] = QChar(0x25A1);
        }

        else if(firstPlayerField[i] == '1')
        {
            formatFirstPlayerField[i] = QChar(0x25A0);
        }
    }

    for(int i = 0; i < secondFieldSize; i++)
    {
        if(secondPlayerField[i] == '0')
        {
            formatSecondPlayerField[i] = QChar(0x25A1);
        }

        else if(secondPlayerField[i] == '1')
        {
            formatSecondPlayerField[i] = QChar(0x25A0);
        }
    }

    // Привязка значения к конкретному полю
    query_->bindValue(":player1",     gameIt->getClientStartedIt()->login_);
    query_->bindValue(":player2",     gameIt->getClientAcceptedIt()->login_);
    query_->bindValue(":field_text1", formatFirstPlayerField);
    query_->bindValue(":field_text2", formatSecondPlayerField);
    query_->bindValue(":start_date",  gameIt->startDate_.toString("yyyy-MM-dd") + gameIt->startTime_.toString("hh:mm:ss"));
    query_->bindValue(":end_date",    gameIt->endDate_.toString("yyyy-MM-dd") + gameIt->endTime_.toString("hh:mm:ss"));
    query_->bindValue(":winner",      gameIt->winnerLogin_);

    // Консольный тест
    qDebug() << gameIt->getClientStartedIt()->login_;
    qDebug() << gameIt->getClientAcceptedIt()->login_;
    qDebug() << formatFirstPlayerField;
    qDebug() << formatSecondPlayerField;
    qDebug() << gameIt->startDate_.toString("yyyy-MM-dd") << gameIt->startTime_.toString("hh:mm:ss");
    qDebug() << gameIt->endDate_.toString("yyyy-MM-dd")   << gameIt->endTime_.toString("hh:mm:ss");
    qDebug() << gameIt->winnerLogin_;

    // Выполнение подготовленного запроса
    query_->exec();
    qDebug() << "New game result pushed to database!";
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
