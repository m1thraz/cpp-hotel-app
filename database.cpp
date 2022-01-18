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
void Database::loginQuery(const int id, const std::string passwort) {
    QSqlQuery query;
    query.prepare("SELECT Passwort FROM Mitarbeiter WHERE MitarbeiterID = :mitarbeiter_id;");
    query.bindValue(":mitarbeiter_id", id);
    bool queryStatus = query.exec();
    qDebug() << "Abfrage erfolgreich: " << queryStatus;
    while(query.next()) {
        if(query.value("Passwort").toString().toStdString() == passwort) {

            //HIER NUN EIN NEUES WELCOME FENSTER STATTDESSEN ÖFFNEN!
            std::cout << "Welcome" << std::endl;
        }
    }
}

//Erstellt die Datenbanktabellen
void Database::createDatabaseTables() {
    QString query = "CREATE TABLE Mitarbeiter ("
                    "MitarbeiterID INT NOT NULL UNIQUE,"
                    "Vorname VARCHAR(250) NOT NULL,"
                    "Nachname VARCHAR(250) NOT NULL,"
                    "Passwort VARCHAR(250) NOT NULL,"
                    "PRIMARY KEY(MitarbeiterID));";
    QSqlQuery exequery;
    bool creationStatus = exequery.exec(query);
    // !Auch false wenn es die Tabellen bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Leere Tabellen wurden erstellt: " << creationStatus;

    createDatabaseEntries();
}

void Database::createDatabaseEntries() {
    QString query = "INSERT INTO Mitarbeiter (MitarbeiterID, Vorname, Nachname, Passwort)"
                    "VALUES (12345, 'Sandra', 'Müller', 'myPassword'), (67890, 'David', 'Tischler', 'asd');";
    QSqlQuery exequery;
    bool creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge wurden erstellt: " << creationStatus;
}

