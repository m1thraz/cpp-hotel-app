#include "invoicecreation.h"
#include "ui_invoicecreation.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"
#include "displayinvoice.h"

invoiceCreation::invoiceCreation(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::invoiceCreation)
{
  ui->setupUi(this);
}

invoiceCreation::~invoiceCreation()
{
  delete ui;
}

void invoiceCreation::on_rabattSlider_sliderMoved(int position) {
  ui->rabattBar->setValue(position);
}

void invoiceCreation::on_rabattAnRadio_toggled() {
  ui->rabattSlider->setEnabled(true);
  ui->rabattSlider->show();
}

void invoiceCreation::on_rabattAusRadio_toggled() {
  ui->rabattSlider->setDisabled(true);
  ui->rabattBar->setValue(0);
  ui->rabattSlider->setValue(0);
}

void invoiceCreation::on_pushButtonRechnungErstellen_clicked() {
  if(!lineEditVerification(1)) {
      return;
    }

  errormessage error;
  if(!this->getBuchungsID()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  //Buchung existiert nicht
  if(!verify.verifyBuchungsIDExists(this->getBuchungsID())) {
      return;
    }

  // Kunde ist nicht ausgecheckt
  if(!verify.verifyKundeIsCheckedOut(this->getBuchungsID())) {
      error.changeTextNotCheckedOut();
      error.setModal(true);
      error.exec();
      return;
    }

  // Rechnung existiert bereits
  if(verify.verifyRechnungExists(this->getBuchungsID())) {
      error.changeTextRechnungExists();
      error.setModal(true);
      error.exec();
      return;
    }

  // Rechnung erstellen wird eingeleitet
  bool rabattAn = ui->rabattAnRadio->isChecked();
  int rabatt;
  if(rabattAn) {
      rabatt = ui->rabattBar->value();
    }else {
      rabatt = 0;
    }

  QSqlQuery query;
  std::string sql;
  QString insert;
  bool queryStatus;

  sql = "INSERT INTO Rechnung (BuchungsID, Rabatt, Rechnungsvermerk, Übernachtungskosten_gesamt, "
        "Sonderleistungskosten_gesamt) "
        "VALUES (" + std::to_string(this->getBuchungsID()) +
      ", " + std::to_string(rabatt) + ", 'offen', "
                                      "(SELECT Übernachtungskosten FROM Zimmerrechnung WHERE BuchungsID = " +
      std::to_string(this->getBuchungsID()) + "), "
                                              "(SELECT SUM(Gesamtkosten) FROM Sonderleistungsrechnung WHERE BuchungsID = " +
      std::to_string(this->getBuchungsID()) + "));";
  insert = QString::fromStdString(sql);
  query.prepare(insert);
  queryStatus = query.exec();
  qDebug() << "Teilerstellung der Rechnung erfolgreich: " << queryStatus;

  if(!queryStatus) {
      error.changeTextDataCreationError();
      error.setModal(true);
      error.exec();
      return;
    }

  sql = "UPDATE Rechnung "
        "SET Gesamtkosten = (CASE WHEN Sonderleistungskosten_gesamt IS NULL THEN "
        "((100 - Rabatt) * Übernachtungskosten_gesamt / 100) ELSE ((100 - Rabatt) * "
        "(Übernachtungskosten_gesamt + Sonderleistungskosten_gesamt) / 100) END) "
        "WHERE BuchungsID = " + std::to_string(this->getBuchungsID()) + ";";
  insert = QString::fromStdString(sql);
  query.prepare(insert);
  queryStatus = query.exec();
  qDebug() << "Rechnung erfolgreich erstellt: " << queryStatus;

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

void invoiceCreation::on_pushButtonModRechnungUpdaten_clicked() {
  if(!lineEditVerification(2)) {
      return;
    }

  errormessage error;
  if(!this->getBuchungsID() || this->getAnmerkungen().empty()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  //Buchung existiert nicht
  if(!verify.verifyBuchungsIDExists(this->getBuchungsID())) {
      return;
    }

  //Rechnung existiert nicht
  if(!verify.verifyRechnungExists(this->getBuchungsID())) {
      error.changeTextRechnungDoesntExist();
      error.setModal(true);
      error.exec();
      return;
    }

  QSqlQuery query;
  std::string sql;
  QString insert;
  bool queryStatus;

  sql = "UPDATE Rechnung SET Rechnungsvermerk = '" + this->getAnmerkungen() +
      "' WHERE BuchungsID = " + std::to_string(this->getBuchungsID()) + ";";
  insert = QString::fromStdString(sql);
  query.prepare(insert);
  queryStatus = query.exec();
  qDebug() << "Rechnungsanmerkungen erfolgreich modifiziert: " << queryStatus;

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

void invoiceCreation::on_pushButtonRechnungAnzeigen_clicked() {
  if(!lineEditVerification(3)) {
      return;
    }

  errormessage error;
  if(!this->getBuchungsID()) {
      qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
      error.changeTextMissingInputText();
      error.setModal(true);
      error.exec();
      return;
    }

  verifier verify;

  //Buchung existiert nicht
  if(!verify.verifyBuchungsIDExists(this->getBuchungsID())) {
      return;
    }

  //Rechnung existiert nicht
  if(!verify.verifyRechnungExists(this->getBuchungsID())) {
      error.changeTextRechnungDoesntExist();
      error.setModal(true);
      error.exec();
      return;
    }

  QSqlQuery query;
  std::string sql;
  QString select;
  bool queryStatus;

  sql = "SELECT * FROM Rechnung WHERE BuchungsID = "
      + std::to_string(this->getBuchungsID()) + ";";
  select = QString::fromStdString(sql);
  query.prepare(select);
  queryStatus = query.exec();
  qDebug() << "Rechnung erfolgreich abgerufen: " << queryStatus;

  if(!queryStatus) {
      error.changeTextDBRequestError();
      error.setModal(true);
      error.exec();
      return;
    }

  while(query.next() == true) {
      invoicedata.push_back("Rechnungsnummer: ");
      invoicedata.push_back(query.value("Rechnungsnummer").toString().toStdString());
      invoicedata.push_back(", BuchungsID: ");
      invoicedata.push_back(query.value("BuchungsID").toString().toStdString());
      invoicedata.push_back(", Übernachtungskosten für gesamten Aufenthalt: ");
      invoicedata.push_back(query.value("Übernachtungskosten_gesamt").toString().toStdString());
      invoicedata.push_back(", Sonderleistungskosten gesamt: ");
      invoicedata.push_back(query.value("Sonderleistungskosten_gesamt").toString().toStdString());
      invoicedata.push_back(", Rabatthöhe: ");
      invoicedata.push_back(query.value("Rabatt").toString().toStdString());
      invoicedata.push_back(", Gesamtkosten: ");
      invoicedata.push_back(query.value("Gesamtkosten").toString().toStdString());
      invoicedata.push_back(", Rechnungsstatus: ");
      invoicedata.push_back(query.value("Rechnungsvermerk").toString().toStdString());
    }

  std::string checkInvoiceInput;
  for (auto& s:invoicedata) {
      checkInvoiceInput+=s;
    }
  qDebug() << "Folgende Daten wurden gespeichert: " << checkInvoiceInput.c_str();

  //    invoicedisplay = new displayinvoice(this->invoicedata);
  //    invoicedisplay->show();
  //    invoicedisplay->changeTextInvoiceInfo();
}

void invoiceCreation::setInvoivedata(std::vector<std::string> &) {
  this->invoicedata = invoicedata;
}

std::vector<std::string> invoiceCreation::getInvoicedata() {
  return this->invoicedata;
}

bool invoiceCreation::lineEditVerification(const int buttontyp) {
  QString tempBuchungsID;
  QString tempAnmerkungen;

  switch(buttontyp) {
    case 1:
      tempBuchungsID = ui->comboBoxNeuRID->currentText();
      break;
    case 2:
      tempBuchungsID = ui->comboBoxModBid->currentText();
      tempAnmerkungen = ui->lineEditModAnmerkung->text();
      break;
    case 3:
      tempBuchungsID = ui->comboBoxAbrfBid->currentText();
      break;
    default:
      qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
      return false;
    }

  // Anfängliche Überprüfung aller textuellen Eingaben
  QRegularExpression numbers("[0-9]+$");
  QRegularExpressionMatch match;
  errormessage error;

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

  //Kein Match für Anmerkungen. Hier werden alle Zeichen erlaubt
  if(!tempAnmerkungen.isEmpty() ) {
      this->setAnmerkungen(tempAnmerkungen.toStdString());
    }

  //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
  return true;
}

void invoiceCreation::setBuchungsID(int buchungsID) {
  this->buchungsID = buchungsID;
}

int invoiceCreation::getBuchungsID() {
  return this->buchungsID;
}

void invoiceCreation::setAnmerkungen(std::string anmerkungen) {
  this->anmerkungen = anmerkungen;
}

std::string invoiceCreation::getAnmerkungen() {
  return this->anmerkungen;
}

