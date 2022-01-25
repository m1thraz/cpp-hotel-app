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
    if(lineEditVerification(1)) {
        errormessage error;
        if(!this->getKundenID() || !this->getMitarbeiterID() || !this->getBestandID()) {
            qDebug() << "Mindestens ein LineEdit Textfeld ist leer";
            error.changeTextMissingInputText();
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

          if(!queryStatus) {
              error.changeTextDBRequestError();
              error.setModal(true);
              error.exec();
         }else if(!exists) {
             error.changeTextKundenIDDoesntExist();
             error.setModal(true);
             error.exec();

          }else {
             //Überprüft, ob es die MitarbeiterID X bereits gibt
             QSqlQuery query;
             query.prepare("SELECT 1 FROM Mitarbeiter WHERE MitarbeiterID = :mitarbeiterID;");
             query.bindValue(":mitarbeiterID", this->getMitarbeiterID());
             queryStatus = query.exec();
             qDebug() << "DB-Überprüfung der MitarbeiterID erfolgreich: " << queryStatus;
             bool exists = false;

             //Wird nur ausgeführt, wenn es die MitarbeiterID tatsächlich gibt, sonst Fehlermeldung
             while(query.next()) {
                exists = true;
             }

             if(!queryStatus) {
                 error.changeTextDBRequestError();
                 error.setModal(true);
                 error.exec();
            }else if(!exists) {
                error.changeTextMitarbeiterIDDoesntExist();
                error.setModal(true);
                error.exec();

             }else {
                 //Überprüft, ob es die BestandID X bereits gibt
                 QSqlQuery query;
                 query.prepare("SELECT 1 FROM Zimmerbestand WHERE BestandID = :bestandID;");
                 query.bindValue(":bestandID", this->getBestandID());
                 queryStatus = query.exec();
                 qDebug() << "DB-Überprüfung der BestandID erfolgreich: " << queryStatus;
                 bool exists = false;

                 //Wird nur ausgeführt, wenn es die MitarbeiterID tatsächlich gibt, sonst Fehlermeldung
                 while(query.next()) {
                    exists = true;
                 }

                 if(!queryStatus) {
                     error.changeTextDBRequestError();
                     error.setModal(true);
                     error.exec();
                }else if(!exists) {
                    error.changeTextBestandIDDoesntExist();
                    error.setModal(true);
                    error.exec();
                }else {
                     //HIER FEHLT NOCH ÜBERPRÜFUNG, OB DAS ZIMMER ÜBERHAUPT FREI IST!!!
                     QSqlQuery query;
                     std::string sql;
                     sql = ("SELECT Anreisedatum, Abreisedatum FROM Zimmerbuchungsliste WHERE (Anreisedatum BETWEEN '" +
                            this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "' OR Abreisedatum BETWEEN '" +
                            this->getAnreiseDatum() + "' AND '" + this->getAbreiseDatum() + "') OR (Anreisedatum < '" +
                            this->getAnreiseDatum() + "' AND Abreisedatum > '" + this->getAbreiseDatum() + "') "
                            "AND BestandID = " + std::to_string(this->getBestandID()) + ";");
                    QString verify = QString::fromStdString(sql);
                    query.prepare(verify);
                    queryStatus = query.exec();
                    qDebug() << "Abfrage der Zimmerbuchungsliste erfolgreich: " << queryStatus;
                    bool exists = false;

                    //Wird nur ausgeführt, wenn das Zimmer in dem Zeitraum bereits gebucht ist, sonst Fehlermeldung
                    while(query.next()) {
                       exists = true;
                    }

                   if(!queryStatus) {
                       error.changeTextDBRequestError();
                       error.setModal(true);
                       error.exec();
                   }else if(exists) {
                       error.changeTextRoomIsBooked();
                       error.setModal(true);
                       error.exec();
                    }else {
                         sql = "INSERT OR IGNORE INTO Zimmerbuchungsliste (BestandID, MitarbeiterID, "
                                 "KundenID, Anreisedatum, Abreisedatum) "
                                 "VALUES (" + std::to_string(this->getBestandID()) + ", " + std::to_string(this->getMitarbeiterID()) + ", "
                                 + std::to_string(this->getKundenID()) + ", '" + this->getAnreiseDatum() + "', '"
                                 + this->getAbreiseDatum() + "');";

                         QString insert = QString::fromStdString(sql);
                         query.prepare(insert);
                         queryStatus = query.exec();
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
                 }
             }
          }
       }
    }
}

void bookroomview::on_bookExtrasButton_clicked() {
    if(lineEditVerification(2)) {

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
