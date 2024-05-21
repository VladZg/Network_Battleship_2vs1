#ifndef FIGHTSHISTORYWINDOW_H
#define FIGHTSHISTORYWINDOW_H

#include <QWidget>

namespace Ui {
class FightsHistoryWindow;
}

class FightsHistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FightsHistoryWindow(QWidget *parent = nullptr);
    ~FightsHistoryWindow();

private:
    Ui::FightsHistoryWindow *ui;
};

#endif // FIGHTSHISTORYWINDOW_H
