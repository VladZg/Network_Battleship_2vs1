#include "fightshistorywindow.h"
#include "ui_fightshistorywindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

FightsHistoryWindow::FightsHistoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FightsHistoryWindow)
{
    ui->setupUi(this);

    setFixedSize(1201, 700);
    setWindowTitle("История сражений");
    setEnabled(false);

    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->setTextElideMode(Qt::TextElideMode::ElideMiddle); // Установка режима обрезки текста как RichText

    QStringList headerLabels;
    headerLabels << "игрок 1" << "игрок 2" << "поле 1" << "поле 2" << "время начала" << "время окончания" << "победитель";
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->setRowCount(0);

    ui->tableWidget->setColumnCount(7); // login1, login2, field1, field2, startTime, endTime, winnerLogin
    ui->tableWidget->setColumnWidth(0, 180);
    ui->tableWidget->setColumnWidth(1, 180);
    ui->tableWidget->setColumnWidth(2, 180);
    ui->tableWidget->setColumnWidth(3, 180);
    ui->tableWidget->setColumnWidth(4, 150);
    ui->tableWidget->setColumnWidth(5, 150);
    ui->tableWidget->setColumnWidth(6, 180);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->tableWidget);
    setLayout(layout);
}

FightsHistoryWindow::~FightsHistoryWindow()
{
    delete ui;
}

void FightsHistoryWindow::addGameEmdingStr(QString& gameEndingStr, int rowIndex)
{
    int currentRowCount = ui->tableWidget->rowCount();
//    ui->tableWidget->setRowCount(currentRowCount + 1);
    ui->tableWidget->setRowHeight(rowIndex, 200);

    QStringList gameInfo = gameEndingStr.split(":");
    qDebug() << "gameInfo: " << gameInfo << " , size = " << gameInfo.size();

    QString str = "";

    QString field1 = gameInfo[2];
    QLabel* field1Label = new QLabel;
    QTableWidgetItem* field1Item = new QTableWidgetItem;
    qDebug() << "field1: " << field1;
    for (int i = 0; i < field1.length(); i++)
    {
        if (i%10 == 0) str.push_back("\n");
        str.push_back(field1[i]);
    }
    qDebug() << "field1: " << str << " , size=" << str.length();
    field1Label->setText(str);
    field1Label->setWordWrap(true);
    field1Item->setData(Qt::DisplayRole, str);

    str.clear();

    QString field2 = gameInfo[3];
    QLabel* field2Label = new QLabel;
    QTableWidgetItem* field2Item = new QTableWidgetItem;
    qDebug() << "field2: " << field2;
    for (int i = 0; i < field2.length(); i++)
    {
        if (i%10 == 0) str.push_back("\n");
        str.push_back(field2[i]);
    }
    qDebug() << "field2: " << str << " , size=" << field2.length();
    field1Label->setText(field2);
    field1Label->setWordWrap(true);
    field1Item->setData(Qt::DisplayRole, str);

    ui->tableWidget->setItem(rowIndex, 0, new QTableWidgetItem(gameInfo[0])); // игрок 1
    ui->tableWidget->setItem(rowIndex, 1, new QTableWidgetItem(gameInfo[1])); // игрок 2
    ui->tableWidget->setItem(rowIndex, 2, field1Item                       ); // поле 1
    ui->tableWidget->setItem(rowIndex, 3, field2Item                       ); // поле 2
    ui->tableWidget->setItem(rowIndex, 4, new QTableWidgetItem(gameInfo[4])); // время начала
    ui->tableWidget->setItem(rowIndex, 5, new QTableWidgetItem(gameInfo[5])); // время окончания
    ui->tableWidget->setItem(rowIndex, 6, new QTableWidgetItem(gameInfo[6])); // победитель
}

void FightsHistoryWindow::fillTable(QStringList& gameEndingsStrList)
{
    ui->tableWidget->clear();
    QStringList headerLabels;
    headerLabels << "игрок 1" << "игрок 2" << "поле 1" << "поле 2" << "время начала" << "время окончания" << "победитель";
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    int nRows = gameEndingsStrList.size();
    ui->tableWidget->setRowCount(nRows);

    for (int rowIndex = nRows-1; rowIndex >= 0; rowIndex--)
    {
        addGameEmdingStr(gameEndingsStrList[rowIndex], rowIndex);
    }
}
