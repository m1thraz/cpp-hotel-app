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
