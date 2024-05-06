#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    model_->insertRow(model_->rowCount());
    model_->submitAll();
}


void MainWindow::on_pushButton_2_clicked()
{
    model_->removeRow(row);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}

//void MainWindow::rotate_90_deg()
//{

//}

//void MainWindow::rotate_180_deg()
//{
//    rotate_90_deg
//}
