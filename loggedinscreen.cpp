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

void LoggedInScreen::on_hotelButton_clicked() {
  hotel = new hotelDatabaseView(this);
  hotel -> show();
}

void LoggedInScreen::on_dbButton_clicked() {
  costumer = new costumerDatabaseView(this);
  costumer -> show();
}

void LoggedInScreen::on_RechnungButton_clicked() {
  invoice = new invoiceCreation(this);
  invoice -> show();
}

void LoggedInScreen::on_logoutButto_clicked() {
  this -> hide();
}

void LoggedInScreen::on_checkInBtn_clicked() {
    checkIn = new checkInCheckOutScreen(this);
    checkIn -> show();
}

void LoggedInScreen::on_buchenButton_clicked() {
    book = new bookroomview(this);
    book->show();
}
