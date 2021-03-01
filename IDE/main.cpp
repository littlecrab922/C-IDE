#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("IDE");
    w.resize(1200, 1000);
    w.show();

    return a.exec();
}
