#include <QApplication>
#include "server.hpp"
#include "mainwindow.hpp"
#include "dbwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication server(argc, argv);

    MainWindow window(12345);
//    DBWindow dbWindow;

//    dbWindow.show();
    window.show();

    return server.exec();
}
