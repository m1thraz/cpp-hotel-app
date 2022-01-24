﻿#include "costumerdatabaseview.h"
#include "ui_costumerdatabaseview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"


costumerDatabaseView::costumerDatabaseView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::costumerDatabaseView)
{
    ui->setupUi(this);
}

costumerDatabaseView::~costumerDatabaseView()
{
    delete ui;
}

void costumerDatabaseView::on_pushButtonSuchen_clicked() {
    if(lineEditVerification(1)) {

    }
}

void costumerDatabaseView::on_pushButtonAktualisieren_clicked() {
    if(lineEditVerification(2)) {
        errormessage error;
        if(!this->getKundenID()) {
            qDebug() << "Keine KundenID angegeben";
            error.changeTextKundenIDError();
            error.setModal(true);
            error.exec();
        }else if(this->getNachname().empty() && this->getVorname().empty() && this->getStrasse().empty() &&
                 !this->getHausnummer() && this->getWohnort().empty() && !this->getPlz() &&
                 !this->getTelefonnummer() && this->getEmail().empty()) {
            qDebug() << "Keine Modifizierungswünsche angegeben";
            error.changeTextMissingCustomerArguments();
            error.setModal(true);
            error.exec();
        }else {
            Database db;

             //Überprüft, ob es die KundenID X bereits gibt
             QSqlQuery query;
             query.prepare("SELECT 1 FROM Kunde WHERE KundenID = :kundenID;");
             query.bindValue(":kundenID", this->getKundenID());
             bool queryStatus = query.exec();
             qDebug() << "DB-Überprüfung der KundenID erfolgreich: " << queryStatus;
             bool exists = false;

             //Wird nur ausgeführt, wenn es die KundenID tatsächlich gibt, sonst Fehlermeldung
             while(query.next()) {
                exists = true;
             }
            if(!exists) {
                error.changeTextKundenIDDoesntExist();
                error.setModal(true);
                error.exec();

            }else {
                std::string sql = "UPDATE Kunde SET ";
                bool firstupdate = true;

                if(!this->getNachname().empty()) {
                    sql += "Nachname = '" + this->getNachname() + "'";
                    firstupdate = false;
                }

                if(!this->getVorname().empty()) {
                    if(firstupdate) {
                        sql += "Vorname = " + this->getVorname();
                        firstupdate = false;
                    }else {
                        sql += ", Vorname = " + this->getVorname();
                    }
                }

                if(!this->getStrasse().empty()) {
                    if(firstupdate) {
                        sql += "Straße = " + this->getStrasse();
                        firstupdate = false;
                    } else {
                        sql += ", Straße = " + this->getStrasse();
                    }
                }

                if(this->getHausnummer()) {
                    if(firstupdate) {
                        sql += "Hausnummer = " + std::to_string(this->getHausnummer());
                        firstupdate = false;
                    }else {
                        sql += ", Hausnummer = " + std::to_string(this->getHausnummer());
                    }
                }

                if(!this->getWohnort().empty()) {
                    if(firstupdate) {
                        sql += "Wohnort = " + this->getWohnort();
                        firstupdate = false;
                    }else {
                        sql += ", Wohnort = " + this->getWohnort();
                    }
                }

                if(this->getPlz()) {
                    if(firstupdate) {
                        sql += "PLZ = " +std::to_string(this->getPlz());
                        firstupdate = false;
                    }else {
                        sql += ", PLZ = " + std::to_string(this->getPlz());
                    }
                }

                if(this->getTelefonnummer()) {
                    if(firstupdate) {
                        sql += "Telefonnummer = " + std::to_string(this->getTelefonnummer());
                        firstupdate = false;
                    }else {
                        sql += ", Telefonnummer = " + std::to_string(this->getTelefonnummer());
                    }
                }

                if(!this->getEmail().empty()) {
                    if(firstupdate) {
                        sql += "'E-Mail' = " + this->getEmail();
                        firstupdate = false;
                    }else {
                        sql += ", 'E-Mail' = " + this->getEmail();
                    }
                }

                sql += " WHERE KundenID = " + std::to_string(this->getKundenID()) + ";";

                QString update = QString::fromStdString(sql);
                qDebug() << "QString: " << update;
                query.prepare(update);
               // query.bindValue(":sql", update);
                queryStatus = query.exec();
                qDebug() << "Update der Kundendaten erfolgreich: " << queryStatus;

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
        }
    }
}

void costumerDatabaseView::on_pushButtonNeuerEintrag_clicked() {
    if(lineEditVerification(3)) {

    }
}

bool costumerDatabaseView::lineEditVerification(const int buttontyp) {
    QString tempKundenID;
    QString tempNachname;
    QString tempVorname;
    QString tempStrasse;
    QString tempHausnummer;
    QString tempWohnort;
    QString tempPLZ;
    QString tempTelenummer;
    QString tempEmail;

    switch(buttontyp) {
    case 1:
        tempKundenID = ui->lineEditAbfrKundenID->text();
        tempNachname = ui->lineEditAbfrNachname->text();
        tempVorname =  ui->lineEditAbfrVorname->text();
        tempStrasse = ui->lineEditAbfrStrasse->text();
        tempHausnummer = ui->lineEditAbfrHausnummer->text();
        tempWohnort = ui->lineEditAbfrWohnort->text();
        tempPLZ = ui->lineEditAbfrPostleitzahl->text();
        tempTelenummer = ui->lineEditAbfrTelefonnummer->text();
        tempEmail = ui->lineEditAbfrEmail->text();
        break;
    case 2:
        tempKundenID = ui->lineEditModKundenID->text();
        tempNachname = ui->lineEditModNachname->text();
        tempVorname =  ui->lineEditModVorname->text();
        tempStrasse = ui->lineEditModStrasse->text();
        tempHausnummer = ui->lineEditModHausnummer->text();
        tempWohnort = ui->lineEditModWohnort->text();
        tempPLZ = ui->lineEditModPostleitzahl->text();
        tempTelenummer = ui->lineEditModTelefonnummer->text();
        tempEmail = ui->lineEditModEmail->text();
        break;
    case 3:
        tempKundenID = ui->lineEditNeuKundenID->text();
        tempNachname = ui->lineEditNeuNachname->text();
        tempVorname =  ui->lineEditNeuVorname->text();
        tempStrasse = ui->lineEditNeuStrasse->text();
        tempHausnummer = ui->lineEditNeuHausnummer->text();
        tempWohnort = ui->lineEditNeuWohnort->text();
        tempPLZ = ui->lineEditNeuPostleitzahl->text();
        tempTelenummer = ui->lineEditNeuTelefonnummer->text();
        tempEmail = ui->lineEditNeuEmail->text();
        break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }

    //Anfängliche Überprüfung aller textuellen Eingaben
    QRegularExpression numbers("[0-9]+$");
    QRegularExpression letters("^[A-z]+$");
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

    match = letters.match(tempStrasse);
    if(!tempStrasse.isEmpty() && match.hasMatch()) {
        this->setStrasse(tempStrasse.toStdString());
    }else if(!tempStrasse.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Straßennamen-Format";
        error.changeTextStrasseWrong();
        error.setModal(true);
        error.exec();
        return false;
    }

    match = numbers.match(tempHausnummer);
    if(!tempHausnummer.isEmpty() && match.hasMatch()) {
        this->setHausnummer(std::stoi(tempHausnummer.toStdString()));
    }else if(!tempHausnummer.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Hausnummer-Format";
        error.changeTextHausnummerWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setHausnummer(0);
    }

    match = letters.match(tempWohnort);
    if(!tempWohnort.isEmpty() && match.hasMatch()) {
        this->setWohnort(tempWohnort.toStdString());
    }else if(!tempWohnort.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Wohnort-Format";
        error.changeTextWohnortWrong();
        error.setModal(true);
        error.exec();
        return false;
    }

    match = numbers.match(tempPLZ);
    if(!tempPLZ.isEmpty() && match.hasMatch()) {
        this->setPlz(std::stoi(tempPLZ.toStdString()));
    }else if(!tempPLZ.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges PLZ-Format";
        error.changeTextPlzWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setPlz(0);
    }

    match = numbers.match(tempTelenummer);
    if(!tempTelenummer.isEmpty() && match.hasMatch()) {
        this->setTelefonnummer(std::stoi(tempTelenummer.toStdString()));
    }else if(!tempTelenummer.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Telefonnummer-Format";
        error.changeTextTelefonnummerWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setTelefonnummer(0);
    }

    //Kein Match für E-Mail. Hier werden alle Zeichen erlaubt
    if(!tempEmail.isEmpty() ) {
        this->setEmail(tempEmail.toStdString());
    }

    //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
    return true;
}

void costumerDatabaseView::setKundenID(int kundenID) {
    this->kundenID = kundenID;
}

int costumerDatabaseView::getKundenID() {
    return this->kundenID;
}

void costumerDatabaseView::setNachname(std::string nachname) {
    this->nachname = nachname;
}

std::string costumerDatabaseView::getNachname() {
    return this->nachname;
}

void costumerDatabaseView::setVorname(std::string vorname) {
    this->vorname = vorname;
}

std::string costumerDatabaseView::getVorname() {
    return this->vorname;
}

void costumerDatabaseView::setStrasse(std::string strasse) {
    this->strasse = strasse;
}

std::string costumerDatabaseView::getStrasse() {
    return this->strasse;
}

void costumerDatabaseView::setHausnummer(int hausnummer) {
    this->hausnummer = hausnummer;
}

int costumerDatabaseView::getHausnummer() {
    return this->hausnummer;
}

void costumerDatabaseView::setWohnort(std::string wohnort) {
    this->wohnort = wohnort;
}

std::string costumerDatabaseView::getWohnort() {
    return this->wohnort;
}

void costumerDatabaseView::setPlz(int plz) {
    this->plz = plz;
}

int costumerDatabaseView::getPlz() {
    return this->plz;
}

void costumerDatabaseView::setTelefonnummer(int telefonnummer) {
    this->telefonnummer = telefonnummer;
}

int costumerDatabaseView::getTelefonnummer() {
    return this->telefonnummer;
}

void costumerDatabaseView::setEmail(std::string email) {
    this->email = email;
}

std::string costumerDatabaseView::getEmail() {
    return this->email;
}
