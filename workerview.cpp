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
void workerview::setMID2(int mID) {
  this->mID = mID;
}

int workerview::getMID2(){
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




bool workerview::lineEditVerification(const int buttontyp){
  QString tempMID;
  QString tempMIDDel;
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
    case 2:
      tempMID = ui->lineEditMID_2->text();
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

  //  match = numbers.match(tempMIDDel);
  //  if(!tempMIDDel.isEmpty() && match.hasMatch()) {
  //      this->setMID2(std::stoi(tempMIDDel.toStdString()));
  //  }else if(!tempMIDDel.isEmpty() && !match.hasMatch()) {
  //      qDebug() << "Ungültiges MitarbeiterID-Format";
  //      error.changeTextMitarbeiterIDWrong();
  //      error.setModal(true);
  //      error.exec();
  //      return false;
  //  }else {
  //      this->setMID2(0);
  //  }


  return true;


}
void workerview::on_pushButtonNeuerEintrag_clicked()
{
  if(!lineEditVerification(1)) {
      return;
    }

  errormessage error;
  if(!this->getMID() || this->getNachname().empty() || this->getVorname().empty()) {
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    } else {




      Database db;
      std::string sql ="INSERT OR IGNORE INTO Mitarbeiter(Nachname, Vorname, MitarbeiterID, Passwort)"
                       "VALUES ('" + this->getNachname() + "', '" + this->getVorname()
          + "', '"  + std::to_string(this->getMID()) + "', '" + this->getPW()

          + "');";

      QString insert = QString::fromStdString(sql);
      QSqlQuery query;
      query.prepare(insert);
      bool queryStatus = query.exec();
      qDebug() << "Hinzufügen der Mitarbeiterdaten erfolgreich: " << queryStatus;

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
}


void workerview::on_pushButtonLoeschen_clicked()
{
  if(!lineEditVerification(2)) {

      return;

    }
  errormessage error;
  if(!this->getMID()) {
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    } else {
      Database db;
      std::string sql ="DELETE FROM Mitarbeiter WHERE MitarbeiterID = "
          + std::to_string(this->getMID()) + ";";

      QString insert = QString::fromStdString(sql);
      QSqlQuery query;
      query.prepare(insert);
      bool queryStatus = query.exec();
      qDebug() << "Löschen der Mitarbeiterdaten erfolgreich: " << queryStatus;

      if(!queryStatus) {
          error.changeTextDeleteDataError();
          error.setModal(true);
          error.exec();
        }else {
          infomessage info;
          info.changeTextDelete();
          info.setModal(true);
          info.exec();
        }
    }
}
