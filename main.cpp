#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *windowObj = new MainWindow();
    windowObj->show();

    return a.exec();
}
