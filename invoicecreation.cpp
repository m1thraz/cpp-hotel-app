#include "invoicecreation.h"
#include "ui_invoicecreation.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"

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
    qDebug() << QString::fromStdString(std::to_string(this->getBuchungsID()));
    qDebug() << QString::fromStdString(this->getAnmerkungen());
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
    qDebug() << "Rechnung erfolgreich erstellt: " << queryStatus;

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
}

bool invoiceCreation::lineEditVerification(const int buttontyp) {
    QString tempBuchungsID;
    QString tempAnmerkungen;

    switch(buttontyp) {
    case 1:
        tempBuchungsID = ui->lineEditNeuBuchungsID->text();
        break;
    case 2:
        tempBuchungsID = ui->lineEditModBuchungsID->text();
        tempAnmerkungen = ui->lineEditModAnmerkung->text();
        break;
    case 3:
        tempBuchungsID = ui->lineEditAbfrBuchungsID->text();
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
