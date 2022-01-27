#include "bookroomview.h"
#include "ui_bookroomview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"

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

    if(!verifyKundenIDExists()) {
        return;
    }

    if(!verifyMitarbeiterIDExists()) {
        return;
    }

    if(!verifyBestandIDExists()) {
        return;
    }

    if(verifyRoomBooked()) {
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
    if(!this->getKundenID() || !this->getMitarbeiterID()) {
        qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
        return;
    }

    if(!verifyKundenIDExists()) {
        return;
    }

    if(!verifyMitarbeiterIDExists()) {
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
    if(lineEditVerification(1)) {
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

    QSqlQuery query;
    std::string sql;

    //Buchung wird storniert, wenn existent
    sql = "DELETE FROM Zimmerbuchungsliste WHERE KundenID = " + std::to_string(this->getKundenID())
            + " AND BestandID = " + std::to_string(this->getBestandID())
            + " AND Anreisedatum = '" + this->getAnreiseDatum() + "' AND Abreisedatum = '"
            + this->getAbreiseDatum() + "');";
    QString deletes = QString::fromStdString(sql);
    query.prepare(deletes);
    bool queryStatus = query.exec();
    qDebug() << "Stornieren der Buchung erfolgreich: " << queryStatus;

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

bool bookroomview::lineEditVerification(const int buttontyp) {
    QString tempKundenID;
    QString tempMitarbeiterID;
    QString tempBestandID;
    QString tempAnreiseDatum;
    QString tempAbreiseDatum;

    switch(buttontyp) {
    case 1:
        tempKundenID = ui->LineEditNeuKundenID->text();
        tempMitarbeiterID = ui->LineEditNeuMitarbeiterID->text();
        tempBestandID = ui->LineEditNeuZimmerID->text();
        tempAnreiseDatum = ui->dateEditNeuAnreiseDatum->text();
        tempAbreiseDatum = ui->dateEditNeuAbreiseDatum->text();
        break;
    case 2:
        tempKundenID = ui->lineEditSonderKundenID->text();
        tempMitarbeiterID = ui->lineEditSonderMitarbeiterID->text();
        break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }

    //Anfängliche Überprüfung aller textuellen Eingaben
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

// Überprüft, ob es die KundenID X bereits gibt
bool bookroomview::verifyKundenIDExists() {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Kunde WHERE KundenID = :kundenID;");
    query.bindValue(":kundenID", this->getKundenID());
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der KundenID erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die KundenID tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextKundenIDDoesntExist();
        error.setModal(true);
        error.exec();
        return false;
    }
}

// Überprüft, ob es die MitarbeiterID X bereits gibt
bool bookroomview::verifyMitarbeiterIDExists() {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Mitarbeiter WHERE MitarbeiterID = :mitarbeiterID;");
    query.bindValue(":mitarbeiterID", this->getMitarbeiterID());
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der MitarbeiterID erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die MitarbeiterID tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextMitarbeiterIDDoesntExist();
        error.setModal(true);
        error.exec();
        return false;
    }
}

// Überprüft, ob es die BestandID X bereits gibt
bool bookroomview::verifyBestandIDExists() {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmerbestand WHERE BestandID = :bestandID;");
    query.bindValue(":bestandID", this->getBestandID());
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der BestandID erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die MitarbeiterID tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextBestandIDDoesntExist();
        error.setModal(true);
        error.exec();
        return false;
    }
}

// Überprüft, ob das Zimmer in Zeitraum X bereits gebucht ist
bool bookroomview::verifyRoomBooked() {
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

    // Wird nur ausgeführt, wenn das Zimmer in dem Zeitraum bereits gebucht ist, da sonst kein query zurück kommt
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

// Bucht Massagen und Sauna
bool bookroomview::bookMasageSauna(int sonderleistungsID) {
    errormessage error;
    QSqlQuery query;
    std::string sql;
    sql = "SELECT 1 FROM GebuchteSonderleistungen WHERE KundenID = " + std::to_string(this->getKundenID())
            + " AND SonderleistungsID = :sonderleistungsID;";
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
              "SonderleistungsID = :sonderleistungsID;";
        QString update = QString::fromStdString(sql);
        query.prepare(update);
        query.bindValue(":sonderleistungsID", sonderleistungsID);
        queryStatus = query.exec();
        qDebug() << "Updaten der gebuchten Sonderleistungen erfolgreich: " << queryStatus;

    // Bucht zum ersten mal Sonderleistungen - Neuer Eintrag muss erzeugt werden
    }else {
        sql = "INSERT INTO GebuchteSonderleistungen (KundenID, MitarbeiterID, "
              "SonderleistungsID, Buchungsanzahl) "
              "VALUES (" + std::to_string(this->getKundenID()) + ", " + std::to_string(this->getMitarbeiterID()) +
                ", :sonderleistungsID, 1);";
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

