#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <iostream>
#include "infologmessage.h"
#include <QRegularExpressionMatch>

// [Hotel Managment Office System] Fenster
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//    second = new hotelDatabaseView();
//    timer = new QTimer();
//    connect(timer, &QTimer::timeout, this, &MainWindow::changeToMainMenu);
//    timer->start(1000);


//    machine = new QStateMachine(this);

//    s1 = new QState();

//    s1-> assignProperty(ui)
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Beim Klick auf den Button "Einloggen" wird das Fenster [Hotel Managment Office System] versteckt.
 * Zusätzlich wird das Fenster [Hotel Managment Office System Fenster - Hauptmenü] geöffnet und angezeigt
 */
void MainWindow::on_loginButton_clicked()
{
    Database db;
    // QLineEdit Textfelder werden eingelesen
    QString tempid = ui->lineEditID->text();
    std::string password = ui->lineEditPassword->text().toStdString();
    // Überprüfung der Mitarbeiter-ID
    QRegularExpression re("[0-9]+");
    QRegularExpressionMatch match = re.match(tempid);
    if(!match.hasMatch()) {
        infologmessage error;
        error.setModal(true);
        error.exec();
    } else {
        int id = std::stoi(tempid.toStdString());
        // Hauptmenü wird nur geöffnet, wenn Logindaten mit Daten aus der Datenbank übereinstimmen
        if(db.loginQuery(id, password)) {
            this -> hide();
            LoggedInScreen w2;
            w2.setModal(true);
            w2.exec();
        }
    }
}

void MainWindow::changeToMainMenu(){

}
//void MainWindow::on_loginButton_clicked()
//{
//    //This is mysql should be checked if this is correct with our database that we need to use!
//    //Connecting to Database
//    QSqlDatabase db;
//    db = QSqlDatabase::addDatabase("QMYSQL", "MyConnect");
//    db.setHostName("localhost");
//    db.setUserName("root");
//    db.setPassword("");
//    db.setDatabaseName("qt5register");

//    QString username = ui->usernameLogin->text();
//    QString password = ui->passwordLogin->text();

//    //Checking if the connection to database is already there
//    if(db.open()) {
//         //Creating My Queries
//        QMessageBox::information(this, "Database Success", "Database Connection Success");

//        QSqlQuery query(QSqlDatabase::database("MyConnect"));

//        //Checking if the input of user is same as the information in the database
//        query.prepare(QString("SELECT * FROM users WHERE username = :username AND password = :password"));

//        //Binding the values that has been given by the users to the varibles for the programm
//        query.bindValue(":username", username);
//        query.bindValue(":password", password);

//        //checking if the query can be executed
//        if(!query.exec()) {

//            QMessageBox::information(this, "Failed", "Query failed to execute");
//        } else {

//            while(query.next()) {

//                QString usernameFromDB = query.value(1).toString();
//                QString passwordFromDB = query.value(2).toString();

//                if(usernameFromDB == username && passwordFromDB == password) {

//                    QMessageBox::information(this, "Success", "Login Success");
//                }else {

//                    QMessageBox::information(this, "Failed", "Login Failed");
//                }
//            }
//        }

//    } else {
//        QMessageBox::information(this, "Database Failed", "Database Connection Failed");
//  }
//}

