#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <iostream>
#include "infologmessage.h"

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
            return true;
        }else {
            infologmessage error;
            error.setModal(true);
            error.exec();
            return false;
        }
    }
    infologmessage error;
    error.setModal(true);
    error.exec();
    return false;
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
    // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Leere Tabelle Mitarbeiter wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Zimmer ("
             "ZimmerID INTEGER,"
             "Zimmertyp VARCHAR(250) NOT NULL,"
             "Zimmerkosten INT NOT NULL,"
             "PRIMARY KEY (ZimmerID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Zimmer wurde erstellt: " << creationStatus;

       query = "CREATE TABLE Zimmerzusatz ("
             "ZimmerzusatzID INTEGER,"
             "Zimmerzusatz VARCHAR(250) NOT NULL,"
             "PRIMARY KEY (ZimmerzusatzID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Zimmerzusatz wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Buchungsstatus ("
             "BuchungsstatusID INTEGER,"
             "Status VARCHAR(250) NOT NULL,"
             "PRIMARY KEY (BuchungsstatusID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Buchunngsstatus wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Zimmerbestand ("
             "BestandID INTEGER,"
             "Zimmernummer INT NOT NULL,"
             "ZimmerID INT,"
             "BuchungsstatusID INT,"
             "PRIMARY KEY (BestandID),"
             "FOREIGN KEY (ZimmerID) REFERENCES Zimmer(ZimmerID),"
             "FOREIGN KEY (BuchungsstatusID) REFERENCES Buchungsstatus(BuchungsstatusID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Zimmerbestand wurde erstellt: " << creationStatus;

     query = "CREATE TABLE BestandZusatzliste ("
             "BestandID INT,"
             "ZimmerzusatzID INT,"
             "FOREIGN KEY (BestandID) REFERENCES Zimmerbestand(BestandID),"
             "FOREIGN KEY (ZimmerzusatzID) REFERENCES Zimmerzusatz(ZimmerzusatzID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle BestandZusatzliste wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Kunde ("
             "KundenID INTEGER,"
             "Vorname VARCHAR(250) NOT NULL,"
             "Nachname VARCHAR(250) NOT NULL,"
             "Straße VARCHAR(250) NOT NULL,"
             "Hausnummer INT NOT NULL,"
             "Wohnort VARCHAR(250) NOT NULL,"
             "PLZ INT NOT NULL,"
             "Telefonnummer INT NOT NULL,"
             "'E-Mail' VARCHAR(250) NOT NULL,"
             "PRIMARY KEY (KundenID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Kunde wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Sonderleistung ("
             "SonderleistungsID INTEGER,"
             "Sonderleistung VARCHAR(250) NOT NULL,"
             "Sonderleistungskosten INT,"
             "PRIMARY KEY (SonderleistungsID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Sonderleistung wurde erstellt: " << creationStatus;

     query = "CREATE TABLE GebuchteSonderleistungen ("
             "KundenID INT,"
             "MitarbeiterID INT,"
             "SonderleistungsID INT,"
             "FOREIGN KEY (KundenID) REFERENCES Kunde(KundenID),"
             "FOREIGN KEY (MitarbeiterID) REFERENCES Mitarbeiter(MitarbeiterID),"
             "FOREIGN KEY (SonderleistungsID) REFERENCES Sonderleistung(SonderleistungsID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle GebuchteSonderleistungen wurde erstellt: " << creationStatus;

     query = "CREATE TABLE Zimmerbuchungsliste ("
             "ZimmerbuchungsID INTEGER,"
             "BestandID INT,"
             "MitarbeiterID INT,"
             "KundenID INT,"
             "Anreisedatum DATE,"
             "Abreisedatum DATE,"
             "PRIMARY KEY (ZimmerbuchungsID),"
             "FOREIGN KEY (BestandID) REFERENCES Zimmerbestand(BestandID),"
             "FOREIGN KEY (MitarbeiterID) REFERENCES Mitarbeiter(MitarbeiterID),"
             "FOREIGN KEY (KundenID) REFERENCES Kunde(KundenID));";
     creationStatus = exequery.exec(query);
     // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
     qDebug()<< "Leere Tabelle Zimmerbuchungsliste wurde erstellt: " << creationStatus;

    createDatabaseEntries();
}

void Database::createDatabaseEntries() {
    QString query = "INSERT INTO Mitarbeiter (MitarbeiterID, Vorname, Nachname, Passwort)"
                    "VALUES (12345, 'Lotte', 'Schalotte', 'myPassword'), (67890, 'Findus', 'Griffel', 'asd');";
    QSqlQuery exequery;
    bool creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Mitarbeiter wurden erstellt: " << creationStatus;

    query = "INSERT INTO Zimmer (Zimmertyp, Zimmerkosten)"
            "VALUES ('Einzelzimmer', 25), ('Doppelzimmer', 30);";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Zimmer wurden erstellt: " << creationStatus;

    query = "INSERT INTO Zimmerzusatz (Zimmerzusatz)"
            "VALUES ('gute Aussicht'), ('Fahrstuhlnähe'), ('Schlafsofa');";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Zimmerzusatz wurden erstellt: " << creationStatus;

    query ="INSERT INTO Buchungsstatus (Status)"
           "VALUES ('verfügbar'), ('gebucht');";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Buchungsstatus wurden erstellt: " << creationStatus;

    query = "INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID, BuchungsstatusID)"
            "VALUES (101, 1, 2), (102, 2, 1), (103, 1, 1), (104, 2, 2);";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Zimmerbestand wurden erstellt: " << creationStatus;

    query = "INSERT INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
            "VALUES (1, 1), (1, 2), (2, 3), (3, null), (4, 1), (4, 2), (4, 3);";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle BestandZusatzliste wurden erstellt: " << creationStatus;

    query = "INSERT INTO Kunde (Vorname, Nachname, Straße, Hausnummer, Wohnort, PLZ, Telefonnummer, 'E-Mail')"
            "VALUES ('Jean', 'White', 'Secondstreet', 2, 'Humbletown', 22904, 1524569876, 'wjean@gmail.com'),"
            "('George', 'Washington', 'Wallstreet', 5, 'Washington', 11111, 4567890156, 'gton@gmail.com');";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Kunde wurden erstellt: " << creationStatus;

    query = "INSERT INTO Zimmerbuchungsliste (BestandID, MitarbeiterID, KundenID, Anreisedatum, Abreisedatum)"
            "VALUES (1, 12345, 1, '2022-08-06', '2022-08-10'), (4, 67890, 2, '2022-08-07', '2022-08-30');";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Zimmerbuchungsliste wurden erstellt: " << creationStatus;

    query = "INSERT INTO Sonderleistung (Sonderleistung, Sonderleistungskosten)"
            "VALUES ('Massagen', 10), ('Sauna', 5);";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle Sonderleistung wurden erstellt: " << creationStatus;

    query = "INSERT INTO GebuchteSonderleistungen (SonderleistungsID, MitarbeiterID, KundenID)"
            "VALUES (1, 12345, 2), (2, 12345, 2);";
    creationStatus = exequery.exec(query);
    // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
    qDebug()<< "Tabelleneinträge für Tabelle GebuchteSonderleistungen wurden erstellt: " << creationStatus;

}

