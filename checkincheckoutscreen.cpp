#include "checkincheckoutscreen.h"
#include "ui_checkincheckoutscreen.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"

checkInCheckOutScreen::checkInCheckOutScreen(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::checkInCheckOutScreen)
{
  ui->setupUi(this);
}

checkInCheckOutScreen::~checkInCheckOutScreen()
{
  delete ui;
}



void checkInCheckOutScreen::on_checkInButton_clicked() {
  if(!lineEditVerification(1)) {
      return;
    }

  errormessage error;
  if(!this->getKundenID() || !this->getBuchungsID()) {
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  // KundenID existiert nicht
  if(!verify.verifyKundenIDExists(this->getKundenID())) {
      return;
    }

  // Die Buchung auf KundenID X existert nicht
  if(!verify.verifyBuchungExists(this->getBuchungsID(), this->getKundenID())) {
      return;
    }

  // Kunde hat bereits eingecheckt
  if(verify.verifyKundeIsCheckedIn(this->getBuchungsID())) {
      error.changeTextCheckInStatus();
      error.setModal(true);
      error.exec();
      return;
    }

  // Kunde hat bereits ausgecheckt
  if(verify.verifyKundeIsCheckedOut(this->getBuchungsID())) {
      error.changeTextCheckOutStatus();
      error.setModal(true);
      error.exec();
      return;
    }

  // Einchecken wird eingeleitet
  QSqlQuery query;

  query.prepare("UPDATE Zimmerbuchungsliste SET BuchungsstatusID = 2 "
                "WHERE BuchungsID = :buchung_buchungsID;");
  query.bindValue(":buchung_buchungsID", this->getBuchungsID());
  bool queryStatus = query.exec();
  qDebug() << "Einchecken erfolgreich: " << queryStatus;

  if(!queryStatus) {
      error.changeTextUpdateError();
      error.setModal(true);
      error.exec();
    }else {
      infomessage info;
      info.changeTextModifiziert();
      info.setModal(true);
      info.exec();
    }
}

void checkInCheckOutScreen::on_checkOutBtn_clicked() {
  if(!lineEditVerification(2)) {
      return;
    }

  errormessage error;
  if(!this->getKundenID() || !this->getBuchungsID()) {
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  // KundenID existiert nicht
  if(!verify.verifyKundenIDExists(this->getKundenID())) {
      return;
    }

  // Buchung auf Kunden X existiert nicht
  if(!verify.verifyBuchungExists(this->getBuchungsID(), this->getKundenID())) {
      return;
    }

  // Der Kunde hat bereits ausgecheckt
  if(verify.verifyKundeIsCheckedOut(this->getBuchungsID())) {
      error.changeTextCheckOutStatus();
      error.setModal(true);
      error.exec();
      return;
    }

  // Der Kunde hat noch nicht eingecheckt
  if(!verify.verifyKundeIsCheckedIn(this->getBuchungsID())) {
      error.changeTextNotCheckedIn();
      error.setModal(true);
      error.exec();
      return;
    }

  bool beschaedigt = ui->checkBoxOutBeschaedigt->isChecked();
  bool reinigung = ui->checkBoxOutReinigung->isChecked();

  if(beschaedigt && reinigung) {
      error.changeTextRepairNeeded();
      error.setModal(true);
      error.exec();
      return;
    }

  // Auschecken wird eingeleitet
  QString anmerkungen;
  if(beschaedigt) {
      anmerkungen = "besch??digte Gegenst??nde";
    }else if(reinigung) {
      anmerkungen = "ben??tigt Reinigung";
    }

  QSqlQuery query;

  query.prepare("UPDATE Zimmerbuchungsliste SET Anmerkungen = :buchung_anmerkungen, BuchungsstatusID = 3 "
                "WHERE BuchungsID = :buchung_buchungsID;");
  query.bindValue(":buchung_anmerkungen", anmerkungen);
  query.bindValue(":buchung_buchungsID", this->getBuchungsID());
  bool queryStatus = query.exec();
  qDebug() << "Auschecken erfolgreich: " << queryStatus;

  if(!queryStatus) {
      error.changeTextUpdateError();
      error.setModal(true);
      error.exec();
    }else {
      infomessage info;
      info.changeTextModifiziert();
      info.setModal(true);
      info.exec();
    }
}

bool checkInCheckOutScreen::lineEditVerification(const int buttontyp) {
  QString tempKundenID;
  QString tempBuchungsID;

  switch(buttontyp) {
    case 1:
      tempKundenID = ui->comboBoxInKID->currentText();
      tempBuchungsID = ui->comboBoxInBID->currentText();
      break;
    case 2:
      tempKundenID = ui->comboBoxOutKID->currentText();
      tempBuchungsID = ui->comboBoxOutBID->currentText();
      break;
    default:
      qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
      return false;
    }

  // Anf??ngliche ??berpr??fung aller textuellen Eingaben
  QRegularExpression numbers("[0-9]+$");
  QRegularExpressionMatch match;
  errormessage error;

  match = numbers.match(tempKundenID);
  if(!tempKundenID.isEmpty() && match.hasMatch()) {
      this->setKundenID(std::stoi(tempKundenID.toStdString()));
    }else if(!tempKundenID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ung??ltiges KundenID-Format";
      error.changeTextKundenIDWrong();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setKundenID(0);
    }

  match = numbers.match(tempBuchungsID);
  if(!tempBuchungsID.isEmpty() && match.hasMatch()) {
      this->setBuchungsID(std::stoi(tempBuchungsID.toStdString()));
    }else if(!tempBuchungsID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ung??ltiges BuchungsID-Format";
      error.changeTextBuchungsIDError();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setBuchungsID(0);
    }

  //Entweder alle Felder leer, oder ??berpr??fungen mit positivem Ergebnis
  return true;
}

void checkInCheckOutScreen::setKundenID(int kundenID) {
  this->kundenID = kundenID;
}

int checkInCheckOutScreen::getKundenID() {
  return this->kundenID;
}

void checkInCheckOutScreen::setBuchungsID(int buchungsID) {
  this->buchungsID = buchungsID;
}

int checkInCheckOutScreen::getBuchungsID() {
  return this->buchungsID;
}


