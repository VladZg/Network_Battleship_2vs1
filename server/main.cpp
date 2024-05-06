#include <QApplication>
#include "server.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window(12345);
    window.show();

    return a.exec();
}
