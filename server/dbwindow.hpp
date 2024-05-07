#ifndef DBWINDOW_HPP
#define DBWINDOW_HPP

#include <QMainWindow>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace DBUi {
    class DBWindow;
} // namespace DBUi

class DBWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DBWindow(QWidget *parent = nullptr);
    ~DBWindow();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);

private:
    DBUi::DBWindow* ui;
    QSqlDatabase db_;
    QSqlQuery *query_;
    QSqlTableModel *model_;

    int row;
};

#endif // DBWINDOW_HPP
