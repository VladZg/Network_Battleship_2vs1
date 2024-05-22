#ifndef FIGHTSHISTORYWINDOW_H
#define FIGHTSHISTORYWINDOW_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class FightsHistoryWindow;
}

class FightsHistoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FightsHistoryWindow(QWidget *parent = nullptr);
    ~FightsHistoryWindow();
    void addGameEmdingStr(QString& gameEndingStr, int rowIndex);
    void fillTable(QStringList& gameEndingsStrList);


private:
    Ui::FightsHistoryWindow *ui;
};

#endif // FIGHTSHISTORYWINDOW_H
