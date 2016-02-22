#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w(nullptr, argv[0]);
    w.show();

    return a.exec();
}
