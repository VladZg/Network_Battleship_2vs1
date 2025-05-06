#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window("127.0.1.1", 50000); //"127.0.0.1", 12345);   // "127.0.0.1",
    window.show();

    return app.exec();
}
