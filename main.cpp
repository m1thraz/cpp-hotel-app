#include "loginscreen.h"

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LoginScreen w;
    w.show();
    return a.exec();
}
