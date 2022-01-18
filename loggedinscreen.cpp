#include "loggedinscreen.h"
#include "ui_loggedinscreen.h"

LoggedInScreen::LoggedInScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggedInScreen)
{
    ui->setupUi(this);
}

LoggedInScreen::~LoggedInScreen()
{
    delete ui;
}

void LoggedInScreen::on_hotelButton_clicked()
{
//  this -> hide();

//  hotelDatabaseView w2;
//  w2.setModal(true);
//  w2.exec();

  hotel = new hotelDatabaseView(this);
  hotel -> show();
}

void LoggedInScreen::on_dbButton_clicked()
{
//  this -> hide();

//  costumerDatabaseView w2;
//  w2.setModal(true);
//  w2.exec();

  costumer = new costumerDatabaseView(this);
  costumer -> show();
}

void LoggedInScreen::on_RechnungButton_clicked()
{
//  this -> hide();

//  invoiceCreation w2;
//  w2.setModal(true);
//  w2.exec();

  invoice = new invoiceCreation(this);
  invoice -> show();
}

void LoggedInScreen::on_logoutButto_clicked()
{
  this -> hide();

//  MainWindow w;
//  w.show();

}

void LoggedInScreen::on_checkInBtn_clicked()
{
    checkIn = new checkInCheckOutScreen(this);
    checkIn -> show();
}
