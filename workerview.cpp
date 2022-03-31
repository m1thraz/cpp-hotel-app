#include "workerview.h"
#include "ui_workerview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include <iostream>




workerview::workerview(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::workerview)
{
  ui->setupUi(this);
}

workerview::~workerview()
{
  delete ui;
}

void workerview::setMID(int mID) {
    this->mID = mID;
}

int workerview::getMID(){
  return this->mID;
}

void workerview::setNachname(std::string nachname) {
    this->nachname = nachname;
}

std::string workerview::getNachname() {
    return this->nachname;
}

void workerview::setVorname(std::string vorname) {
    this->vorname = vorname;
}

std::string workerview::getVorname() {
    return this->vorname;
}

void workerview::setPW(std::string pw) {
    this->pw = pw;
}

std::string workerview::getPW() {
    return this->pw;
}



void workerview::on_buttonBox_accepted()
{

}
bool workerview::lineEditVerification(const int buttontyp){
  QString tempMID;
  QString tempPW;
  QString tempVorname;
  QString tempNachname;

  switch(buttontyp){
    case 1:
      tempVorname = ui->lineEditVorname->text();
      tempNachname = ui->lineEditNachname->text();
      tempMID = ui->lineEditMID->text();
      tempPW = ui->lineEditPW->text();
      break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }
  QRegularExpression numbers("[0-9]+$");
  QRegularExpression letters("^[A-z]+$");
  QRegularExpressionMatch match;
  errormessage error;

  match = numbers.match(tempMID);
  if(!tempMID.isEmpty() && match.hasMatch()) {
      this->setMID(std::stoi(tempMID.toStdString()));
  }else if(!tempMID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges MitarbeiterID-Format";
      error.changeTextMitarbeiterIDWrong();
      error.setModal(true);
      error.exec();
      return false;
  }else {
      this->setMID(0);
  }
  match = letters.match(tempNachname);
  if(!tempNachname.isEmpty() && match.hasMatch()) {
      this->setNachname(tempNachname.toStdString());
  }else if(!tempNachname.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges Nachnamen-Format";
      error.changeTextNachnameWrong();
      error.setModal(true);
      error.exec();
      return false;
  }
  match = letters.match(tempVorname);
  if(!tempVorname.isEmpty() && match.hasMatch()) {
      this->setVorname(tempVorname.toStdString());
  }else if(!tempVorname.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges Vornamen-Format";
      error.changeTextVornameWrong();
      error.setModal(true);
      error.exec();
      return false;
  }
  match = letters.match(tempPW);
  if(!tempPW.isEmpty() && match.hasMatch()) {
      this->setPW(tempPW.toStdString());
  }else if(!tempPW.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges Passwort-Format";
      error.changeTextPWWrong();
      error.setModal(true);
      error.exec();
      return false;
  }




return true;


}
void workerview::on_pushButtonNeuerEintrag_clicked()
{
  errormessage error;
  Database db;
  std::string sql ="INSERT OR IGNORE INTO Mitarbeiter(Nachname, Vorname, MitarbeiterID, Passwort)"
                   "VALUES ('" + this->getNachname() + "', '" + this->getVorname()
         + "', '"  + std::to_string(this->getMID()) + "', " + this->getPW()

        + "');";

  QString insert = QString::fromStdString(sql);
  QSqlQuery query;
  query.prepare(insert);
  bool queryStatus = query.exec();

  if(!queryStatus) {
      error.changeTextDataCreationError();
      error.setModal(true);
      error.exec();
    }else {
      infomessage info;
      info.changeTextNeu();
      info.setModal(true);
      info.exec();
    }
}
