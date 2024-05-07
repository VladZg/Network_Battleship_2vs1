#include "dbwindow.hpp"
#include "ui_dbwindow.h"

namespace DBUi {
    class DBWindow: public Ui_DBWindow {};
} // namespace Ui

DBWindow::DBWindow(QWidget *parent)
    : QMainWindow{parent},
      ui(new DBUi::DBWindow)
{
    ui->setupUi(this);
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

    ui->tableView->setModel(model_);
}

DBWindow::~DBWindow()
{
    delete ui;
}

void DBWindow::on_addButton_clicked()
{
    model_->insertRow(model_->rowCount());
    model_->submitAll();
}


void DBWindow::on_removeButton_clicked()
{
    model_->removeRow(row);
}


void DBWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}
