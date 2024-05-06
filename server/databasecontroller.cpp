#include "databasecontroller.h"

DBController::DBController(QWidget *parent) :  QDialog(parent), m_ui(new Ui::DBController)
{
    m_ui->setupUi(this);

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName("./testDB.db");
    if(db_.open())
    {
        qDebug("open");
    }
    else
    {
        qDebug("no open");
    }

    query_ = new QSqlQuery(db_);
    query_->exec("CREATE TABLE FuckBook(Boyname TEXT, Girlname TEXT, Date TEXT);");

    model_ = new QSqlTableModel(this, db_);
    model_->setTable("FuckBook");
    model_->select();

    ui_->tableView->setModel(model_);

}

DBController::~DBController()
{
    delete m_ui;
}
