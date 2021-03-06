#include "bookroomview.h"
#include "ui_bookroomview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"

bookroomview::bookroomview(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::bookroomview)
{
  ui->setupUi(this);


}

bookroomview::~bookroomview()
{
  delete ui;
}

void bookroomview::on_bookRoomButton_clicked() {
  if(!lineEditVerification(1)) {
      return;
    }
  errormessage error;
  if(!this->getKundenID() || !this->getMitarbeiterID() || !this->getBestandID()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  if(!verify.verifyKundenIDExists(this->getKundenID())) {
      return;
    }

  if(!verify.verifyMitarbeiterIDExists(this->getMitarbeiterID())) {
      return;
    }

  if(!verify.verifyBestandIDExists(this->getBestandID())) {
      return;
    }

  if(verifyRoomShouldNotBeBooked()) {
      return;
    }

  QSqlQuery query;
  std::string sql;

  //Buchungsprozess wird eingeleitet
  sql = "INSERT INTO Zimmerbuchungsliste (BestandID, MitarbeiterID, "
        "KundenID, Anreisedatum, Abreisedatum, BuchungsstatusID) "
        "VALUES (" + std::to_string(this->getBestandID()) + ", " + std::to_string(this->getMitarbeiterID()) + ", "
      + std::to_string(this->getKundenID()) + ", '" + this->getAnreiseDatum() + "', '"
      + this->getAbreiseDatum() + "', 1);";

  QString insert = QString::fromStdString(sql);
  query.prepare(insert);
  bool queryStatus = query.exec();
  qDebug() << "Hinzufügen der Buchung erfolgreich: " << queryStatus;

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

void bookroomview::on_bookExtrasButton_clicked() {
  if(!lineEditVerification(2)) {
      return;
    }
  errormessage error;
  if(!this->getKundenID() || !this->getMitarbeiterID() || !this->getBuchungsID()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  if(!verify.verifyKundenIDExists(this->getKundenID())) {
      return;
    }

  if(!verify.verifyMitarbeiterIDExists(this->getMitarbeiterID())) {
      return;
    }

  if(!verify.verifyBuchungExists(this->getBuchungsID(), this->getKundenID())) {
      return;
    }

  if(!verify.verifyKundeIsCheckedIn(this->getBuchungsID())) {
      return;
    }

  bool massagen = ui->checkBoxSonderMassage->isChecked();
  bool sauna = ui->checkBoxSonderSauna->isChecked();

  if(!massagen && !sauna) {
      error.changeTextMissingSpecialService();
      error.setModal(true);
      error.exec();
    }

  // Sonder-Buchungsprozess für Massagen wird eingeleitet
  if(massagen) {
      if(!bookMasageSauna(1)) {
          return;
        }
    }

  // Sonder-Buchungsprozess für Sauna wird eingeleitet
  if(sauna) {
      bookMasageSauna(2);
    }
}

void bookroomview::on_cancelBookedRoomButton_clicked() {
  if(!lineEditVerification(1)) {
      return;
    }
  errormessage error;
  if(!this->getKundenID() || !this->getBestandID()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  if(!verify.verifyKundenIDExists(this->getKundenID())) {
      return;
    }

  if(!verify.verifyBestandIDExists(this->getBestandID())) {
      return;
    }

  if(!verifyRoomShouldBeBooked()) {
      return;
    }

  QSqlQuery query;
  std::string sql;

  //Buchung wird storniert, wenn existent
  sql = "DELETE FROM Zimmerbuchungsliste WHERE KundenID = " + std::to_string(this->getKundenID())
      + " AND BestandID = " + std::to_string(this->getBestandID())
      + " AND Anreisedatum = '" + this->getAnreiseDatum() + "' AND Abreisedatum = '"
      + this->getAbreiseDatum() + "';";
  QString deletes = QString::fromStdString(sql);
  query.prepare(deletes);
  bool queryStatus = query.exec();
  qDebug() << "Stornieren der Buchung erfolgreich: " << queryStatus;

  if(!queryStatus) {
      error.changeTextDeleteDataError();
      error.setModal(true);
      error.exec();//  if(!lineEditVerification(2)) {

      // return;

      //}
      //  errormessage error;
      //  if(!this->getMID2()) {
      //      error.changeTextMissingInputText();
      //      error.setModal(true);
      //      error.exec();
      //      return;
      //  } else {
      //Database db;
      //std::string sql ="DELETE FROM Mitarbeiter(MitarbeiterID)"
      //    "VALUES ('" + std::to_string(this->getMID2()) + "');";

      //QString insert = QString::fromStdString(sql);
      //QSqlQuery query;
      //query.prepare(insert);
      //bool queryStatus = query.exec();
      // qDebug() << "Löschen der Mitarbeiterdaten erfolgreich: " << queryStatus;

      //if(!queryStatus) {
      //    error.changeTextDataCreationError();
      //    error.setModal(true);
      //    error.exec();
      //  }else {
      //    infomessage info;
      //    info.changeTextNeu();
      //    info.setModal(true);
      //    info.exec();
      //  }
      //    }
    }else {
      infomessage info;
      info.changeTextDelete();
      info.setModal(true);
      info.exec();
    }
}

bool bookroomview::lineEditVerification(const int buttontyp) {
  QString tempKundenID;
  QString tempMitarbeiterID;
  QString tempBestandID;
  QString tempAnreiseDatum;
  QString tempAbreiseDatum;
  QString tempBuchungsID;

  switch(buttontyp) {
    case 1:
      tempKundenID = ui-> comboBoxKID->currentText();
      tempMitarbeiterID = ui->LineEditNeuMitarbeiterID->text();
      tempBestandID = ui->comboBoxBeID->currentText();
      tempAnreiseDatum = ui->dateEditNeuAnreiseDatum->text();
      tempAbreiseDatum = ui->dateEditNeuAbreiseDatum->text();
      break;
    case 2:
      tempKundenID = ui->comboBoxSondKID->currentText();
      tempMitarbeiterID = ui->lineEditSonderMitarbeiterID->text();
      tempBuchungsID = ui->comboBoxSonBID->currentText();
      break;
    default:
      qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
      return false;
    }

  // Anfängliche Überprüfung aller textuellen Eingaben
  QRegularExpression numbers("[0-9]+$");
  QRegularExpressionMatch match;
  errormessage error;

  match = numbers.match(tempKundenID);
  if(!tempKundenID.isEmpty() && match.hasMatch()) {
      this->setKundenID(std::stoi(tempKundenID.toStdString()));
    }else if(!tempKundenID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges KundenID-Format";
      error.changeTextKundenIDWrong();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setKundenID(0);
    }

  match = numbers.match(tempMitarbeiterID);
  if(!tempMitarbeiterID.isEmpty() && match.hasMatch()) {
      this->setMitarbeiterID(std::stoi(tempMitarbeiterID.toStdString()));
    }else if(!tempMitarbeiterID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges MitarbeiterID-Format";
      error.changeTextMitarbeiterIDWrong();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setMitarbeiterID(0);
    }

  match = numbers.match(tempBestandID);
  if(!tempBestandID.isEmpty() && match.hasMatch()) {
      this->setBestandID(std::stoi(tempBestandID.toStdString()));
    }else if(!tempBestandID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges BestandID-Format";
      error.changeTextBestandIDWrong();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setBestandID(0);
    }

  match = numbers.match(tempBuchungsID);
  if(!tempBuchungsID.isEmpty() && match.hasMatch()) {
      this->setBuchungsID(std::stoi(tempBuchungsID.toStdString()));
    }else if(!tempBuchungsID.isEmpty() && !match.hasMatch()) {
      qDebug() << "Ungültiges BuchungsID-Format";
      error.changeTextBuchungsIDError();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      this->setBuchungsID(0);
    }

  // Kein Match für Anreise und Abreise, da Datumsfeld
  if(!tempAnreiseDatum.isEmpty()) {
      tempAnreiseDatum.replace(".", "-");
      this->setAnreiseDatum(tempAnreiseDatum.toStdString());
    }
  if(!tempAbreiseDatum.isEmpty()) {
      tempAbreiseDatum.replace(".", "-");
      this->setAbreiseDatum(tempAbreiseDatum.toStdString());
    }

  //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
  return true;
}

// Überprüft, ob das Zimmer in Zeitraum X bereits gebucht ist - Für Zimmerbuchung
bool bookroomview::verifyRoomShouldNotBeBooked() {
  errormessage error;
  QSqlQuery query;
  std::string sql;
  sql = ("SELECT 1 FROM Zimmerbuchungsliste WHERE ((Anreisedatum BETWEEN '" +
         this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "' OR Abreisedatum BETWEEN '" +
         this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "') OR (Anreisedatum < '" +
         this->getAnreiseDatum() + "' AND Abreisedatum > '" + this->getAbreiseDatum() + "')) "
                                                                                        "AND BestandID = " + std::to_string(this->getBestandID()) + ";");
  QString verify = QString::fromStdString(sql);
  query.prepare(verify);
  bool queryStatus = query.exec();
  qDebug() << "Abfrage der Zimmerbuchungsliste erfolgreich: " << queryStatus;

  // Es gibt eine Zimmerbuchung, aber es darf kein Zimmer gebucht sein
  if(query.next()) {
      error.changeTextRoomIsBooked();
      error.setModal(true);
      error.exec();
      return true;
    }else if(!queryStatus) {
      error.changeTextDBRequestError();
      error.setModal(true);
      error.exec();
      return true;
    }else {
      return false;
    }
}

// Überprüft, ob die Buchung in Zeitraum X existiert - Für Stornierung
bool bookroomview::verifyRoomShouldBeBooked() {
  errormessage error;
  QSqlQuery query;
  std::string sql;
  sql = ("SELECT 1 FROM Zimmerbuchungsliste WHERE ((Anreisedatum BETWEEN '" +
         this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "' OR Abreisedatum BETWEEN '" +
         this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "') OR (Anreisedatum < '" +
         this->getAnreiseDatum() + "' AND Abreisedatum > '" + this->getAbreiseDatum() + "')) "
                                                                                        "AND BestandID = " + std::to_string(this->getBestandID()) + " AND KundenID = " +
         std::to_string(this->getKundenID()) + ";");
  QString verify = QString::fromStdString(sql);
  query.prepare(verify);
  bool queryStatus = query.exec();
  qDebug() << "Abfrage der Zimmerbuchungsliste erfolgreich: " << queryStatus;

  // Es gibt eine Zimmerbuchung und es muss ein Zimmer gebucht sein
  if(query.next()) {
      return true;
    }else if(!queryStatus) {
      error.changeTextDBRequestError();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      error.changeTextCancelBookingError();
      error.setModal(true);
      error.exec();
      return false;
    }
}

// Bucht Massagen und Sauna
bool bookroomview::bookMasageSauna(int sonderleistungsID) {
  errormessage error;
  QSqlQuery query;
  std::string sql;
  sql = "SELECT 1 FROM GebuchteSonderleistungen WHERE KundenID = " + std::to_string(this->getKundenID())
      + " AND SonderleistungsID = :sonderleistungsID AND "
      + "MitarbeiterID = " + std::to_string(this->getMitarbeiterID())
      + " AND BuchungsID = " + std::to_string(this->getBuchungsID()) +";";
  QString verify = QString::fromStdString(sql);
  query.prepare(verify);
  query.bindValue(":sonderleistungsID", sonderleistungsID);
  bool queryStatus = query.exec();
  qDebug() << "Abfrage der gebuchten Sonderleistungen erfolgreich: " << queryStatus;

  if(!queryStatus) {
      error.changeTextDBRequestError();
      error.setModal(true);
      error.exec();
      return false;
    }

  // Kunde hat bereits Sonderleistung gebucht
  if(query.next()) {
      sql = "UPDATE GebuchteSonderleistungen SET Buchungsanzahl = Buchungsanzahl + 1 "
            "WHERE KundenID = " + std::to_string(this->getKundenID()) + " AND "
                                                                        "SonderleistungsID = :sonderleistungsID AND MitarbeiterID = " + std::to_string(this->getMitarbeiterID())
          + " AND BuchungsID = " + std::to_string(this->getBuchungsID()) + ";";
      QString update = QString::fromStdString(sql);
      query.prepare(update);
      query.bindValue(":sonderleistungsID", sonderleistungsID);
      queryStatus = query.exec();
      qDebug() << "Updaten der gebuchten Sonderleistungen erfolgreich: " << queryStatus;

      // Bucht zum ersten mal Sonderleistungen - Neuer Eintrag muss erzeugt werden
    }else {
      sql = "INSERT INTO GebuchteSonderleistungen (KundenID, BuchungsID, MitarbeiterID, "
            "SonderleistungsID, Buchungsanzahl) "
            "VALUES (" + std::to_string(this->getKundenID()) + ", " + std::to_string(this->getBuchungsID()) + ", "
          + std::to_string(this->getMitarbeiterID()) + ", :sonderleistungsID, 1);";
      QString insert = QString::fromStdString(sql);
      query.prepare(insert);
      query.bindValue(":sonderleistungsID", sonderleistungsID);
      queryStatus = query.exec();
      qDebug() << "Hinzufügen der gebuchten Sonderleistungen erfolgreich: " << queryStatus;
    }

  if(!queryStatus) {
      error.changeTextDataCreationError();
      error.setModal(true);
      error.exec();
      return false;
    }else {
      return true;
    }
}

void bookroomview::setKundenID(int kundenID) {
  this->kundenID = kundenID;
}

int bookroomview::getKundenID() {
  return this->kundenID;
}

void bookroomview::setMitarbeiterID(int mitarbeiterID) {
  this->mitarbeiterID = mitarbeiterID;
}

int bookroomview::getMitarbeiterID() {
  return this->mitarbeiterID;
}

void bookroomview::setBestandID(int bestandID) {
  this->bestandID = bestandID;
}

int bookroomview::getBestandID() {
  return this->bestandID;
}

void bookroomview::setAnreiseDatum(std::string anreisedatum) {
  this->anreisedatum = anreisedatum;
}

std::string bookroomview::getAnreiseDatum() {
  return this->anreisedatum;
}

void bookroomview::setAbreiseDatum(std::string abreisedatum) {
  this->abreisedatum = abreisedatum;
}

std::string bookroomview::getAbreiseDatum() {
  return this->abreisedatum;
}

void bookroomview::setBuchungsID(int buchungsID) {
  this->buchungsID = buchungsID;
}

int bookroomview::getBuchungsID() {
  return this->buchungsID;
}




void bookroomview::setMID(QString MID){
  s = MID;
  ui->LineEditNeuMitarbeiterID->setText(s);
  ui->lineEditSonderMitarbeiterID->setText(s);

}

void bookroomview::on_comboBox_activated(int index)
{
  QSqlQuery query;
  std::string sql;


  sql = "SELECT ALL FROM Kunde";
  query.bindValue(":kundenID", kundenID);
  bool queryStatus = query.exec();
  //QStringList list = NULL;
  //ui->comboBox->addItems(list);
}

