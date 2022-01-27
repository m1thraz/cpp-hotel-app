#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include <iostream>
#include "errormessage.h"
#include <QRegularExpressionMatch>
#include <QDebug>
#include "loggedinscreen.h"

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
    QRegularExpression re("^[0-9]+$");
    QRegularExpressionMatch match = re.match(tempid);
    if(!match.hasMatch()) {
        qDebug() << "Das eingegebene ID-Format ist fehlerhaft";
        errormessage error;
        error.changeTextIDWrong();
        error.setModal(true);
        error.exec();
    } else {
        int id = std::stoi(tempid.toStdString());
        // Hauptmenü wird nur geöffnet, wenn Logindaten mit Daten aus der Datenbank übereinstimmen
        if(db.loginQuery(id, password)) {
            ui->lineEditID->clear();
            ui->lineEditPassword->clear();
            this -> hide();
            LoggedInScreen w2;
            w2.setMainWindow(this);
            w2.setModal(true);
            w2.exec();
        }
    }
}

void MainWindow::changeToMainMenu(){

}
