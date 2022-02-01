#include "verifier.h"
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include <QSqlQuery>

verifier::verifier() {
}

// Überprüft, ob es die KundenID X bereits gibt
bool verifier::verifyKundenIDExists(int kundenID) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Kunde WHERE KundenID = :kundenID;");
    query.bindValue(":kundenID", kundenID);
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
bool verifier::verifyMitarbeiterIDExists(int mitarbeiterID) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Mitarbeiter WHERE MitarbeiterID = :mitarbeiterID;");
    query.bindValue(":mitarbeiterID", mitarbeiterID);
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
bool verifier::verifyBestandIDExists(int bestandID) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmerbestand WHERE BestandID = :bestandID;");
    query.bindValue(":bestandID", bestandID);
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

// Überprüft, ob es das Zimmer mit der Zimmernummer X bereits gibt
bool verifier::verifyZimmernummerExists(int zimmernummer) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
    query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der Zimmernummer erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die Zimmernummer tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextZimmernummerError();
        error.setModal(true);
        error.exec();
        return false;
    }
}

// Überprüft, ob es das Zimmer mit der Zimmernummer X noch nicht gibt
bool verifier::verifyZimmernummerDoesntExist(int zimmernummer) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
    query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der Zimmernummer erfolgreich: " << queryStatus;

    //Zimmer existiert
    if(query.next()) {
        error.changeTextZimmernummerExists();
        error.setModal(true);
        error.exec();
        return false;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        return true;
    }
}

// Überprüft, ob es die ZimmerID X bereits gibt
bool verifier::verifyZimmerIDExists(int zimmerID) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmer WHERE ZimmerID = :zimmer_zimmerID;");
    query.bindValue(":zimmer_zimmerID", zimmerID);
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der ZimmerID erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die ZimmerID tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextZimmerIDError();
        error.setModal(true);
        error.exec();
        return false;
    }
}

// Überprüft, ob die Buchung X vom Kunden Y gibt
bool verifier::verifyBuchungExists(int buchungsID, int kundenID) {
    errormessage error;
    QSqlQuery query;
    query.prepare("SELECT 1 FROM Zimmerbuchungsliste WHERE BuchungsID = :buchung_buchungsID "
                  "AND KundenID = :buchung_kundenID;");
    query.bindValue(":buchung_buchungsID", buchungsID);
    query.bindValue(":buchung_kundenID", kundenID);
    bool queryStatus = query.exec();
    qDebug() << "DB-Überprüfung der Buchung erfolgreich: " << queryStatus;

    //Wird nur ausgeführt, wenn es die Buchung tatsächlich gibt, sonst Fehlermeldung
    if(query.next()) {
        return true;
    }else if(!queryStatus) {
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        error.changeTextBuchungDoesntExist();
        error.setModal(true);
        error.exec();
        return false;
    }
}
