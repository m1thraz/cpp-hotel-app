#include "loggedinscreen.h"
#include "ui_loggedinscreen.h"
#include "mainwindow.h"

LoggedInScreen::LoggedInScreen(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoggedInScreen)
{
  ui->setupUi(this);
  // ui->mitarbeiter->setText("<p>Bitte geben Sie für eine Preisanpassung sowohl den<br>Preis, als auch eine ZimmerID ein.</p>");
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
  mwindow->show();
  this->close();
}

void LoggedInScreen::on_checkInBtn_clicked() {
  checkIn = new checkInCheckOutScreen(this);
  checkIn -> show();
}

void LoggedInScreen::on_buchenButton_clicked() {
  book = new bookroomview(this);
  book->setMID(s);
  book->show();
}

void LoggedInScreen::setMainWindow(MainWindow *mwindow) {
  this->mwindow = mwindow;
}
void LoggedInScreen::setMID(int MID){
  s = QString::number(MID);
  ui->mitarbeiter->setText(s);
}



void LoggedInScreen::on_RechnungButton_2_clicked()
{
  worker = new workerview(this);
  worker->show();
}
