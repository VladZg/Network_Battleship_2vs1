#include "fightshistorywindow.h"
#include "ui_fightshistorywindow.h"

FightsHistoryWindow::FightsHistoryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightsHistoryWindow)
{
    ui->setupUi(this);
}

FightsHistoryWindow::~FightsHistoryWindow()
{
    delete ui;
}
