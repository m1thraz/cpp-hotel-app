#include "mainwindow.h"

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Add new Database to projekt, trys to load the driver for the database
        // db.setHostName("127.0.0.1"); // Loopback-Adress will be added not needed with sqlite!
        db.setDatabaseName("text.db"); // Sets name for the Database

        bool success = db.open(); //Checks wether connection was successfull and saves as success
        qDebug()<<"Database connection established: " <<success; // Gives the text out

        QString sqlEmployee =      "CREATE TABLE employee ("
                                   "ID INT PRIMARY KEY ,"
                                   "name varchar(60)                       ,"
                                   "surname varchar(60)                    ,"
                                   "dateOfBirth int                        ,"
                                   "eMailAdress varchar(90)                ,"
                                   "phoneNumber int                        ,"
                                   "Street varchar(60)                     ,"
                                   "zipCode int                            ,"
                                   "City varchar(60)                       );";

        QString sqlCostumer =      "CREATE TABLE costumer (                 "
                                   "ID INT PRIMARY KEY                     ,"
                                   "name varchar(60)                       ,"
                                   "surname varchar(60)                    ,"
                                   "dateOfBirth int                        ,"
                                   "eMailAdress varchar(90)                ,"
                                   "phoneNumber int                        ,"
                                   "Street varchar(60)                     ,"
                                   "zipCode int                            ,"
                                   "City varchar(60)                       ,"
                                   "hasStayedBefore boolean                ,"
                                   "lastVisit date                         ,"
                                   "paidBill boolean                       );";

        QString sqlRooms =         "CREATE TABLE costumer (                 "
                                   "ID INT PRIMARY KEY                     ,"
                                   "name varchar(60)                       ,"
                                   "size int                               ,"
                                   "numberOfBeds int                       ,"
                                   "windowView varChar(60)                 ,"
                                   "onWichFloor int                        ,"
                                   "price int                              ,"
                                   "reserved boolean                       ,"
                                   "booked boolean                         ,"
                                   "cleaned boolean                        );";

        QSqlQuery query1(db);

        query1.prepare(sqlEmployee);

        bool executeNote1 = query1.exec();
        qDebug()<<"SQL employee statement status: " << executeNote1;

        QSqlQuery query2(db);

        query2.prepare(sqlCostumer);

        bool executeNote2 = query2.exec();
        qDebug()<<"SQL costumer statement status: " << executeNote2;

        QSqlQuery query3(db);

        query3.prepare(sqlRooms);

        bool executeNote3 = query3.exec();
        qDebug()<<"SQL costumer statement status: " << executeNote3;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
