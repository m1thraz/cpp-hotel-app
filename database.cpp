#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <iostream>
#include "errormessage.h"

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
  qDebug() << "Abfrage des Passworts erfolgreich: " << queryStatus;
  // Die angegebene ID existiert. Sonst gibt es kein query
  while(query.next()) {
      // Passwörter stimmen überein
      if(query.value("Passwort").toString().toStdString() == passwort) {
          qDebug()<< "Passwörter stimmen überein";
          return true;
        }else {
          qDebug()<< "Passwörter stimmen nicht überein";
          errormessage error;
          error.changeTextLoginError();
          error.setModal(true);
          error.exec();
          return false;
        }
    }
  // Die angegebene ID existiert nicht.
  qDebug()<< "Die angegebene ID existiert nicht";
  errormessage error;
  error.changeTextLoginError();
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
          "Zimmertyp VARCHAR(250) NOT NULL UNIQUE,"
          "Zimmerkosten INT NOT NULL,"
          "PRIMARY KEY (ZimmerID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Zimmer wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Zimmerzusatz ("
          "ZimmerzusatzID INTEGER,"
          "Zimmerzusatz VARCHAR(250) NOT NULL UNIQUE,"
          "PRIMARY KEY (ZimmerzusatzID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Zimmerzusatz wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Buchungsstatus ("
          "BuchungsstatusID INTEGER,"
          "Status VARCHAR(250) NOT NULL UNIQUE,"
          "PRIMARY KEY (BuchungsstatusID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Buchunngsstatus wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Zimmerbestand ("
          "BestandID INTEGER,"
          "Zimmernummer INT NOT NULL UNIQUE,"
          "ZimmerID INT,"
          "PRIMARY KEY (BestandID),"
          "FOREIGN KEY (ZimmerID) REFERENCES Zimmer(ZimmerID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Zimmerbestand wurde erstellt: " << creationStatus;

  query = "CREATE TABLE BestandZusatzliste ("
          "BestandID INT,"
          "ZimmerzusatzID INT,"
          "FOREIGN KEY (BestandID) REFERENCES Zimmerbestand(BestandID),"
          "FOREIGN KEY (ZimmerzusatzID) REFERENCES Zimmerzusatz(ZimmerzusatzID),"
          "PRIMARY KEY (BestandID, ZimmerzusatzID));";
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
          "Telefonnummer INT NOT NULL UNIQUE,"
          "'E-Mail' VARCHAR(250) NOT NULL UNIQUE,"
          "PRIMARY KEY (KundenID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Kunde wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Sonderleistung ("
          "SonderleistungsID INTEGER,"
          "Sonderleistung VARCHAR(250) NOT NULL UNIQUE,"
          "Sonderleistungskosten INT,"
          "PRIMARY KEY (SonderleistungsID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Sonderleistung wurde erstellt: " << creationStatus;

  query = "CREATE TABLE GebuchteSonderleistungen ("
          "KundenID INT,"
          "BuchungsID INT,"
          "MitarbeiterID INT,"
          "SonderleistungsID INT,"
          "Buchungsanzahl INT,"
          "FOREIGN KEY (KundenID) REFERENCES Kunde(KundenID),"
          "FOREIGN KEY (BuchungsID) REFERENCES Zimmerbuchungsliste(BuchungsID),"
          "FOREIGN KEY (MitarbeiterID) REFERENCES Mitarbeiter(MitarbeiterID),"
          "FOREIGN KEY (SonderleistungsID) REFERENCES Sonderleistung(SonderleistungsID),"
          "PRIMARY KEY (KundenID, BuchungsID, SonderleistungsID, MitarbeiterID));";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle GebuchteSonderleistungen wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Zimmerbuchungsliste ("
          "BuchungsID INTEGER,"
          "BestandID INT,"
          "MitarbeiterID INT,"
          "KundenID INT,"
          "Anreisedatum DATE,"
          "Abreisedatum DATE,"
          "BuchungsstatusID INT,"
          "Anmerkungen VARCHAR(250),"
          "PRIMARY KEY (BuchungsID),"
          "FOREIGN KEY (BestandID) REFERENCES Zimmerbestand(BestandID),"
          "FOREIGN KEY (MitarbeiterID) REFERENCES Mitarbeiter(MitarbeiterID),"
          "FOREIGN KEY (KundenID) REFERENCES Kunde(KundenID),"
          "FOREIGN KEY (BuchungsstatusID) REFERENCES Buchungsstatus(BuchungsstatusID));";

  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Zimmerbuchungsliste wurde erstellt: " << creationStatus;

  query = "CREATE TABLE Rechnung ("
          "Rechnungsnummer INTEGER,"
          "BuchungsID INT UNIQUE,"
          "'Übernachtungskosten_gesamt' INT,"
          "'Sonderleistungskosten_gesamt' INT,"
          "Rabatt INT,"
          "Gesamtkosten DOUBLE,"
          "Rechnungsvermerk VARCHAR(250),"
          "PRIMARY KEY (Rechnungsnummer),"
          "FOREIGN KEY (BuchungsID) REFERENCES Zimmerbuchungsliste(BuchungsID));";

  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Tabelle bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Leere Tabelle Rechnung wurde erstellt: " << creationStatus;

  createDatabaseEntries();
}

void Database::createDatabaseEntries() {
  QString query = "INSERT OR IGNORE INTO Mitarbeiter (MitarbeiterID, Vorname, Nachname, Passwort)"
                  "VALUES (12345, 'Lotte', 'Schalotte', 'myPassword'), (67890, 'Findus', 'Griffel', 'asd');";
  QSqlQuery exequery;
  bool creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Mitarbeiter wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Zimmer (Zimmertyp, Zimmerkosten)"
          "VALUES ('Einzelzimmer', 25), ('Doppelzimmer', 30);";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Zimmer wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Zimmerzusatz (Zimmerzusatz)"
          "VALUES ('gute Aussicht'), ('Fahrstuhlnähe'), ('Schlafsofa');";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Zimmerzusatz wurden erstellt: " << creationStatus;

  query ="INSERT OR IGNORE INTO Buchungsstatus (Status)"
         "VALUES ('reserviert'), ('checked-in'), ('checked-out');";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Buchungsstatus wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Zimmerbestand (Zimmernummer, ZimmerID)"
          "VALUES (101, 1), (102, 2), (103, 1), (104, 2);";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Zimmerbestand wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
          "VALUES (1, 1), (1, 2), (2, 3), (4, 1), (4, 2), (4, 3);";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle BestandZusatzliste wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Kunde (Vorname, Nachname, Straße, Hausnummer, Wohnort, PLZ, Telefonnummer, 'E-Mail')"
          "VALUES ('Jean', 'White', 'Secondstreet', 2, 'Humbletown', 22904, 1524569876, 'wjean@gmail.com'),"
          "('George', 'Washington', 'Wallstreet', 5, 'Washington', 11111, 4567890156, 'gton@gmail.com');";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Kunde wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Zimmerbuchungsliste (BuchungsID, BestandID, MitarbeiterID, KundenID, Anreisedatum, Abreisedatum, "
          "BuchungsstatusID, Anmerkungen)"
          "VALUES (10099, 3, 12345, 1, '2022-01-10', '2022-01-12', 3, null), "
          "(10100, 1, 12345, 1, '2022-03-14', '2022-03-24', 1, null), "
          "(10101, 4, 67890, 2, '2022-01-25', '2022-03-01', 2, 'benötigt Reinigung');";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Zimmerbuchungsliste wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Sonderleistung (Sonderleistung, Sonderleistungskosten)"
          "VALUES ('Massagen', 10), ('Sauna', 5);";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle Sonderleistung wurden erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO GebuchteSonderleistungen (SonderleistungsID, BuchungsID, MitarbeiterID, KundenID, Buchungsanzahl)"
          "VALUES (1, 10101, 12345, 2, 1), (2, 10101, 12345, 2, 4);";
  creationStatus = exequery.exec(query);
  // !Auch false wenn es die Einträge bereits gibt! Dann bitte neuen Build, wenn nötig
  qDebug()<< "Tabelleneinträge für Tabelle GebuchteSonderleistungen wurden erstellt: " << creationStatus;

  query = "CREATE VIEW Hotelzimmer AS "
          "SELECT be.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, "
          "MAX(CASE WHEN be.ZimmerzusatzID = 1 THEN 'true' ELSE 'false' end) AS Aussicht, "
          "MAX(CASE WHEN be.ZimmerzusatzID = 2 THEN 'true' ELSE 'false' end) AS Fahrstuhlnähe, "
          "MAX(CASE WHEN be.ZimmerzusatzID = 3 THEN 'true' ELSE 'false' end) AS Sofa "
          "FROM BestandZusatzliste be "
          "LEFT JOIN Zimmerbestand zi "
          "ON be.BestandID = zi.BestandID "
          "LEFT JOIN Zimmer zim "
          "ON zi.ZimmerID = zim.ZimmerID "
          "GROUP BY be.BestandID;";
  creationStatus = exequery.exec(query);
  qDebug()<< "View::Hotelzimmer wurde erstellt: " << creationStatus;

  query = "CREATE VIEW Sonderleistungsrechnung AS "
          "SELECT ge.BuchungsID, se.Sonderleistung, ge.Buchungsanzahl, "
          "SUM(ge.Buchungsanzahl*se.Sonderleistungskosten) AS 'Gesamtkosten' "
          "FROM GebuchteSonderleistungen ge "
          "LEFT JOIN Sonderleistung se "
          "ON ge.SonderleistungsID = se.SonderleistungsID "
          "GROUP BY ge.BuchungsID, ge.SonderleistungsID "
          "ORDER BY se.Sonderleistung ASC;";
  creationStatus = exequery.exec(query);
  qDebug()<< "View::Sonderleistungsrechnung wurde erstellt: " << creationStatus;

  query = "CREATE VIEW Zimmerrechnung AS "
          "SELECT zi.BuchungsID, zim.Zimmertyp, "
          "julianday(zi.Abreisedatum) - julianday(zi.Anreisedatum) AS Übernachtungstage, "
          "SUM((julianday(zi.Abreisedatum) - julianday(zi.Anreisedatum)) * zim.Zimmerkosten) "
          "AS Übernachtungskosten "
          "FROM Zimmerbuchungsliste zi "
          "LEFT JOIN Zimmerbestand be "
          "ON zi.BestandID = be.BestandID "
          "LEFT JOIN Zimmer zim "
          "ON be.ZimmerID = zim.ZimmerID "
          "GROUP BY BuchungsID;";
  creationStatus = exequery.exec(query);
  qDebug()<< "View::Zimmerrechnung wurde erstellt: " << creationStatus;

  query = "INSERT OR IGNORE INTO Rechnung (Rechnungsnummer, BuchungsID, 'Übernachtungskosten_gesamt', "
          "'Sonderleistungskosten_gesamt', Rabatt, Gesamtkosten, Rechnungsvermerk) "
          "VALUES (500100, 10099, 50, 30, 20, 64, 'offen')";
  creationStatus = exequery.exec(query);
  qDebug()<< "Tabelleneinträge für Tabelle Rechnung wurden erstellt: " << creationStatus;

}

int Database::getBestandID(int zimmernummer) {
  errormessage error;
  QSqlQuery query;
  query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
  query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
  bool queryStatus = query.exec();
  qDebug() << "DB-Abfrage der BestandID erfolgreich: " << queryStatus;

  //Wird nur ausgeführt, wenn es die Zimmernummer tatsächlich gibt, sonst Fehlermeldung
  if(query.next()) {
      int bestandID = std::stoi(query.value("BestandID").toString().toStdString());
      return bestandID;
    }else if(!queryStatus) {
      error.changeTextDBRequestError();
      error.setModal(true);
      error.exec();
    }
}
