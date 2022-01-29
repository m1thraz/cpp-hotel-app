#include "mainwindow.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database db;
    db.connectToDatabase();

    /*
     * Startpunkt für das erste Fenster - Hotel Managment Office System
     * .show öffnet das Fenster
     */
    MainWindow w;
    w.show();
    //db.closeDatabaseConnection();
    return a.exec();
}
