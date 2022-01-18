#include "mainwindow.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database db;
    db.connectToDatabase();
    db.loginQuery(12345, "myPassword");

    /*
     * Startpunkt für das erste Fenster - Hotel Managment Office System
     * .show öffnet das Fenster
     */
    MainWindow w;
    w.show();
    return a.exec();
}
