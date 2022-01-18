#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
     * Startpunkt für das erste Fenster - Hotel Managment Office System
     * .show öffnet das Fenster
     */
    MainWindow w;
    w.show();
    return a.exec();
}
