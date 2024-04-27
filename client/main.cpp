#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window("127.0.0.1", 12345);
    window.show();

    return app.exec();
}
