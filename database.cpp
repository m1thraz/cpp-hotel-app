#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <iostream>

Database::Database() {
}

// Stellt eine Verbindung zur Datenbank [hotel-app] her und erstellt eine SQLITE Datenbank, wenn noch keine existiert
void Database::connectToDatabase() {
    // Initialisierung
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setHostName("127.0.0.1");
    // Pfad zur Datenbank. Im build Ordner zu finden
    database.setDatabaseName("hotel-app.sqlite");

    //Stellt die eigentliche Verbindung her
    bool connectionStatus = database.open();

    qDebug()<< "Datenbankverbindung wurde hergestellt: " << connectionStatus;

    createDatabaseTables();
}

void Database::closeDatabaseConnection() {
    //Entfernt die Verbindung vollständig
    QSqlDatabase::removeDatabase("hotelApp");
}

// Überprüft, ob die Login-Daten stimmen
bool Database::loginQuery(const int id, const std::string passwort) {
    QSqlQuery query;
    query.prepare("SELECT Passwort FROM Mitarbeiter WHERE MitarbeiterID = :mitarbeiter_id;");
    query.bindValue(":mitarbeiter_id", id);
    bool queryStatus = query.exec();
    qDebug() << "Abfrage erfolgreich: " << queryStatus;
    while(query.next()) {
        if(query.value("Passwort").toString().toStdString() == passwort) {

            //HIER NUN STATTDESSEN EIN NEUES WELCOME FENSTER ÖFFNEN oder weglassen, da direkt hauptmenü aufgeht!
            std::cout << "Welcome" << std::endl;
            return true;
        }else {
            //HIER EIN EXTRA FENSTER MIT FALSCHES PASSWORT TEXT - oK
            std::cout << "Falsches Passwort" << std::endl;
            return false;
        }
    }
    //HIER EIN EXTRA FENSTER MIT ID EXISTIERT NICHT TEXT - OK
     std::cout << "ID existiert nicht" << std::endl;
    return false;
}

//Erstellt die Datenbanktabellen
void Database::createDatabaseTables() {
    QString query = "CREATE TABLE Mitarbeiter ("
                    "MitarbeiterID INT NOT NULL UNIQUE,"
                    "Vorname VARCHAR(250) NOT NULL,"
                    "Nachname VARCHAR(250) NOT NULL,"
                    "Passwort VARCHAR(250) NOT NULL,"
                    "PRIMARY KEY(MitarbeiterID));"

                    "CREATE TABLE Zimmer ("
                    "ZimmerID INT NOT NULL AUTOINCREMENT,"
                    "Zimmertyp VARCHAR(250) NOT NULL,"
                    "Zimmerkosten INT NOT NULL"
                    "PRIMARY KEY(ZimmerID));"

                    ;
    QSqlQuery exequery;
    bool creationStatus = exequery.exec(query);
    // !Auch false wenn es die Tabellen bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Leere Tabellen wurden erstellt: " << creationStatus;

    createDatabaseEntries();
}

void Database::createDatabaseEntries() {
    QString query = "INSERT INTO Mitarbeiter (MitarbeiterID, Vorname, Nachname, Passwort)"
                    "VALUES (12345, 'Sandra', 'Müller', 'myPassword'), (67890, 'David', 'Tischler', 'asd');"

                    "INSERT INTO Zimmer (Zimmertyp, Zimmerkosten)"
                    "VALUES ('Einzelzimmer', 25), ('Doppelzimmer, 30);";
    QSqlQuery exequery;
    bool creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge wurden erstellt: " << creationStatus;
}

