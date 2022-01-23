#include "hoteldatabaseview.h"
#include "ui_hoteldatabaseview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <iostream>
#include <QSqlQuery>
#include <QDebug>

hotelDatabaseView::hotelDatabaseView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hotelDatabaseView)
{
    ui->setupUi(this);
}

hotelDatabaseView::~hotelDatabaseView()
{
    delete ui;
}

void hotelDatabaseView::on_aktualisierenButton_clicked() {
    Database db;
     // QLineEdit Textfelder werden eingelesen
    QString tempZimmerID = ui->lineEditModZimmerID->text();
    QString tempPreis = ui->lineEditModPreis->text();
    QString tempZimmernummer = ui->lineEditModZimmernummer->text();

    // Zimmernummer angegeben
    if(!tempZimmernummer.isEmpty()) {

        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch match = re.match(tempZimmernummer);
        if(!match.hasMatch()) {
            //GUI ERROR MESSAGE - Ungültiges Zimmernummerformat. nur zahlen
            std::cout << "Ungültige Zimmernummer" << std::endl;

        }else {
            int zimmernummer = std::stoi(tempZimmernummer.toStdString());
            //Überprüft, ob es das Zimmer mit der Zimmernummer X bereits gibt
            QSqlQuery query;
            query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
            bool queryStatus = query.exec();
            qDebug() << "Abfrage erfolgreich: " << queryStatus;
            bool exists = false;
            //Wird nur ausgeführt, wenn es das Zimmer tatsächlich gibt, sonst Fehlermeldung
            while(query.next()) {
                exists = true;
                int bestandID = std::stoi(query.value("BestandID").toString().toStdString());
                // QCheckBox Felder werden ausgewertet
                bool doppelbett = ui->checkBoxModDoppelbett->isChecked();
                bool einzelbett = ui->checkBoxModEinzelbett->isChecked();
                bool aussicht = ui->checkBoxModAussicht->isChecked();
                bool fahrstuhl = ui->checkBoxModFahrstuhl->isChecked();
                bool sofa = ui->checkBoxModSofa->isChecked();
                bool veraenderung = false;

                if(doppelbett && !einzelbett) {
                    veraenderung = true;
                    query.prepare("UPDATE Zimmerbestand SET ZimmerID = 2 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                    queryStatus = query.exec();
                    qDebug() << "Update Doppelbett erfolgreich: " << queryStatus;
                }else if(einzelbett && !doppelbett) {
                    veraenderung = true;
                    query.prepare("UPDATE Zimmerbestand SET ZimmerID = 1 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                    queryStatus = query.exec();
                    qDebug() << "Update Einzelbett erfolgreich: " << queryStatus;
                }
                else if (doppelbett && einzelbett){
                    veraenderung = true;
                    std::cout << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!" << std::endl;
                }
                if(aussicht) {
                    veraenderung = true;
                    query.prepare("INSERT OR IGNORE INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                  "VALUES (:zimmerbestand_bestandID, 1);");
                    query.bindValue(":zimmerbestand_bestandID", bestandID);
                    queryStatus = query.exec();
                    qDebug() << "Update Aussicht erfolgreich: " << queryStatus;
                }

                if(fahrstuhl) {
                    veraenderung = true;
                    query.prepare("INSERT OR IGNORE INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                  "VALUES (:zimmerbestand_bestandID, 2);");
                    query.bindValue(":zimmerbestand_bestandID", bestandID);
                    queryStatus = query.exec();
                    qDebug() << "Update Fahrstuhl erfolgreich: " << queryStatus;
                }

                if(sofa) {
                    veraenderung = true;
                    query.prepare("INSERT OR IGNORE INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                  "VALUES (:zimmerbestand_bestandID, 3);");
                    query.bindValue(":zimmerbestand_bestandID", bestandID);
                    queryStatus = query.exec();
                    qDebug() << "Update Sofa erfolgreich: " << queryStatus;
                }

                if(!veraenderung) {
                    std::cout << "Bitte geben Sie neben der Zimmernummer die Veränderungswünsche an!" << std::endl;
                }
            }
            if(!exists) {
                std::cout << "Zimmer existiert nicht!" << std::endl;
            }
        }
    }

    // Entweder nur ZimmerID oder Preis angegeben - Führt zu Error
    if((tempZimmerID.isEmpty() && !tempPreis.isEmpty()) ||
             tempPreis.isEmpty() && !tempZimmerID.isEmpty()) {
        /*
         * GUI ERROR MESSAGE - Für das Anpassen eines Zimmerpreises, muss eine ZimmerID eingegeben werden.
         * Das Anpassen einer ZimmerID ist nicht möglich!
         */
        std::cout << "test" << std::endl;

    // ZimmerID und Preis angegeben
    }else if(!tempZimmerID.isEmpty() && !tempPreis.isEmpty()) {
        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch matchZimmerID = re.match(tempZimmerID);
        QRegularExpressionMatch matchPreis = re.match(tempPreis);

        if(!matchZimmerID.hasMatch() || !matchPreis.hasMatch()) {
            //GUI ERROR MESSAGE - Ungültiges Format. nur zahlen
            std::cout << "Ungültiges Format" << std::endl;
        } else {
            std::cout << "Preisanpassung" << std::endl;
            int zimmerID = std::stoi(tempZimmerID.toStdString());
            int preis = std::stoi(tempPreis.toStdString());
            //Überprüft, ob es die ZimmerID bereits gibt
            QSqlQuery query;
            query.prepare("SELECT 1 FROM Zimmer WHERE ZimmerID = :zimmer_zimmerID;");
            query.bindValue(":zimmer_zimmerID", zimmerID);
            bool queryStatus = query.exec();
            qDebug() << "Abfrage erfolgreich: " << queryStatus;
            bool exists = false;

            //Wird nur ausgeführt, wenn es die ZimmerID tatsächlich gibt, sonst Fehlermeldung
            while(query.next()) {
                exists = true;
                query.prepare("UPDATE Zimmer SET Zimmerkosten = :zimmer_preis WHERE ZimmerID = :zimmer_zimmerID;");
                query.bindValue(":zimmer_preis", preis);
                query.bindValue(":zimmer_zimmerID", zimmerID);
                queryStatus = query.exec();
                qDebug() << "Update Preis erfolgreich: " << queryStatus;
            }
            if(!exists) {
                std::cout << "ZimmerID existiert nicht!" << std::endl;
            }
        }
    }
    // Alle Eingabefelder (nicht Checkbox) sind leer
    else {
        std::cout << "nix ausgewählt" << std::endl;
    }
}

void hotelDatabaseView::on_eintragenButton_clicked(){
    Database db;
     // QLineEdit Textfelder werden eingelesen
    QString tempZimmernummer = ui->lineEditNeuZimmernummer->text();

    // Zimmernummer angegeben
    if(!tempZimmernummer.isEmpty()) {

        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch match = re.match(tempZimmernummer);
        if(!match.hasMatch()) {
            //GUI ERROR MESSAGE - Ungültiges Zimmernummerformat. nur zahlen
            std::cout << "Ungültige Zimmernummer" << std::endl;

        }else {
            int zimmernummer = std::stoi(tempZimmernummer.toStdString());
            //Überprüft, ob es das Zimmer mit der Zimmernummer X bereits gibt
            QSqlQuery query;
            query.prepare("SELECT 1 FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
            bool queryStatus = query.exec();
            qDebug() << "Abfrage erfolgreich: " << queryStatus;
            bool exists = false;
            //Wird nur ausgeführt, wenn es das Zimmer tatsächlich gibt, sonst Fehlermeldung
            while(query.next()) {
                 std::cout << "Zimmer existiert bereits!" << std::endl;
                 exists = true;
            }
            if(!exists) {
                bool doppelbett = ui->checkBoxNeuDoppelbett->isChecked();
                bool einzelbett = ui->checkBoxNeuEinzelbett->isChecked();
                bool aussicht = ui->checkBoxNeuAussicht->isChecked();
                bool fahrstuhl = ui->checkBoxNeuFahrstuhl->isChecked();
                bool sofa = ui->checkBoxNeuSofa->isChecked();
                bool ausstattung = false;
                int bestandID;

                if (doppelbett && einzelbett){
                    ausstattung = true;
                    std::cout << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!" << std::endl;
                }else if(!doppelbett && !einzelbett) {
                    ausstattung = true;
                    std::cout << "Bett muss Einzelbett oder Doppelbett sein!" << std::endl;
                }else {
                    if(doppelbett && !einzelbett) {
                        // Erstellen eines neuen Zimmers
                       ausstattung = true;
                        query.prepare("INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID, BuchungsstatusID)"
                                "VALUES (:zimmerbestand_zimmernummer, 2, 1);");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        queryStatus = query.exec();
                        qDebug() << "Erstellen eines Zimmers mit Doppelbett erfolgreich: " << queryStatus;

                        //Abfrage der neuen BestandID
                        query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        bool queryStatus = query.exec();
                        qDebug() << "Abfrage erfolgreich: " << queryStatus;
                        while(query.next()) {
                            bestandID = std::stoi(query.value("BestandID").toString().toStdString());
                        }
                    }else if(einzelbett && !doppelbett) {
                        // Erstellen eines neuen Zimmers
                        ausstattung = true;
                        query.prepare("INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID, BuchungsstatusID)"
                                "VALUES (:zimmerbestand_zimmernummer, 1, 1);");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        queryStatus = query.exec();
                        qDebug() << "Erstellen eines Zimmers mit Einzelbett erfolgreich: " << queryStatus;

                        //Abfrage der neuen BestandID
                        query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        bool queryStatus = query.exec();
                        qDebug() << "Abfrage erfolgreich: " << queryStatus;
                        while(query.next()) {
                            bestandID = std::stoi(query.value("BestandID").toString().toStdString());
                        }
                    }

                    if(aussicht) {
                        ausstattung = true;
                        query.prepare("INSERT INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                      "VALUES (:zimmerbestand_bestandID, 1);");
                        query.bindValue(":zimmerbestand_bestandID", bestandID);
                        queryStatus = query.exec();
                        qDebug() << "Hinzufügen der Aussicht erfolgreich: " << queryStatus;
                    }

                    if(fahrstuhl) {
                        ausstattung = true;
                        query.prepare("INSERT INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                      "VALUES (:zimmerbestand_bestandID, 2);");
                        query.bindValue(":zimmerbestand_bestandID", bestandID);
                        queryStatus = query.exec();
                        qDebug() << "Hinzufügen Fahrstuhl erfolgreich: " << queryStatus;
                    }

                    if(sofa) {
                        ausstattung = true;
                        query.prepare("INSERT INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                                      "VALUES (:zimmerbestand_bestandID, 3);");
                        query.bindValue(":zimmerbestand_bestandID", bestandID);
                        queryStatus = query.exec();
                        qDebug() << "Hinzufügen Sofa erfolgreich: " << queryStatus;
                    }

                    if(!ausstattung) {
                        std::cout << "Bitte geben Sie neben der Zimmernummer die Ausstattungsswünsche an!" << std::endl;
                    }
                }
            }
        }
    // Eingabefeld für Zimmernummer (nicht Checkbox) ist leer
    }else {
        std::cout << "Keine Zimmernummer angegeben" << std::endl;
    }
}

void hotelDatabaseView::on_suchenButton_clicked() {
    Database db;
     // QLineEdit Textfelder werden eingelesen
    QString tempZimmernummer = ui->lineEditAbfrZimmernummer->text();

    // QCheckBox Felder werden ausgewertet
    bool doppelbett = ui->checkBoxAbfrDoppelbett->isChecked();
    bool einzelbett = ui->checkBoxAbfrEinzelbett->isChecked();
    bool aussicht = ui->checkBoxAbfrAussicht->isChecked();
    bool fahrstuhl = ui->checkBoxAbfrFahrstuhl->isChecked();
    bool sofa = ui->checkBoxAbfrSofa->isChecked();
    bool verfuegbar = ui->radioButtonAbfrVerfuegbar->isChecked();

    bool ausstattung = false;
    QString sql;

    if(doppelbett && !einzelbett) {
        ausstattung = true;
        sql = "zi.ZimmerID = 2";
    }else if(einzelbett && !doppelbett) {
        ausstattung = true;
        sql = "zi.ZimmerID = 1";
    }else if(doppelbett && einzelbett){
        ausstattung = true;
        // !Keine WHERE Bedingung bei allen Zimmertypen nötig!
    }

    if(aussicht || fahrstuhl || sofa) {
        sql += " AND ";
    }

    if(aussicht && (fahrstuhl || sofa)) {
        ausstattung = true;
        sql += "(zu.ZimmerzusatzID = 1 OR ";
    }else if(aussicht){
        ausstattung = true;
        sql += "zu.ZimmerzusatzID = 1";
    }

    if(fahrstuhl && sofa) {
        ausstattung = true;
        sql += "zu.ZimmerzusatzID = 2 OR ";
    }
    if(fahrstuhl) {
        ausstattung = true;
        sql += "zu.ZimmerzusatzID = 2";
    }

    if(sofa && (fahrstuhl || aussicht)) {
        ausstattung = true;
        sql += "zu.ZimmerzusatzID = 3)";
    }
    if(sofa) {
        ausstattung = true;
        sql += "zu.ZimmerzusatzID = 3";
    }

    if(verfuegbar) {
        ausstattung = true;
        sql += " AND bu.BuchungsstatusID = 1";
    }

    QSqlQuery query;
    bool queryStatus;

    // Zimmernummer angegeben
    if(!tempZimmernummer.isEmpty()) {

        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch match = re.match(tempZimmernummer);
        if(!match.hasMatch()) {
            //GUI ERROR MESSAGE - Ungültiges Zimmernummerformat. nur zahlen
            std::cout << "Ungültige Zimmernummer" << std::endl;

        }else {
            int zimmernummer = std::stoi(tempZimmernummer.toStdString());
            //Überprüft, ob es das Zimmer mit der Zimmernummer X bereits gibt
            query.prepare("SELECT Zimmernummer FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
            queryStatus = query.exec();
            qDebug() << "Abfrage, ob es die Zimmernummer gibt, erfolgreich: " << queryStatus;
            bool exists = false;

            //Wird nur ausgeführt, wenn es das Zimmer tatsächlich gibt, sonst Fehlermeldung
            while(query.next()) {
                exists = true;
            }
            if(!exists) {
                std::cout << "Zimmer existiert nicht!" << std::endl;
            }else if(!ausstattung) {
                query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, bu.Status, zu.Zimmerzusatz "
                              "FROM Zimmerbestand zi "
                              "LEFT JOIN BestandZusatzliste be "
                              "ON zi.BestandID = be.BestandID "
                              "LEFT JOIN Zimmer zim "
                              "ON zi.ZimmerID = zim.ZimmerID "
                              "LEFT JOIN Buchungsstatus bu "
                              "ON zi.BuchungsstatusID = bu.BuchungsstatusID "
                              "LEFT JOIN Zimmerzusatz zu "
                              "ON be.ZimmerzusatzID = zu.ZimmerzusatzID "
                              "WHERE zi.Zimmernummer = :zimmerbestand_zimmernummer;");
                query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                queryStatus = query.exec();
                qDebug() << "Abfrage mit Zimmer erfolgreich: " << queryStatus;
            }else {
                query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, bu.Status, zu.Zimmerzusatz "
                              "FROM Zimmerbestand zi "
                              "LEFT JOIN BestandZusatzliste be "
                              "ON zi.BestandID = be.BestandID "
                              "LEFT JOIN Zimmer zim "
                              "ON zi.ZimmerID = zim.ZimmerID "
                              "LEFT JOIN Buchungsstatus bu "
                              "ON zi.BuchungsstatusID = bu.BuchungsstatusID "
                              "LEFT JOIN Zimmerzusatz zu "
                              "ON be.ZimmerzusatzID = zu.ZimmerzusatzID "
                              "WHERE zi.Zimmernummer = :zimmerbestand_zimmernummer AND :sql;");
                query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                query.bindValue(":sql", sql);
                queryStatus = query.exec();
                qDebug() << "Abfrage mit Zimmer und Ausstattung erfolgreich: " << queryStatus;
            }
        }
    }else {
        if(!ausstattung) {
            query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, bu.Status, zu.Zimmerzusatz "
                          "FROM Zimmerbestand zi "
                          "LEFT JOIN BestandZusatzliste be "
                          "ON zi.BestandID = be.BestandID "
                          "LEFT JOIN Zimmer zim "
                          "ON zi.ZimmerID = zim.ZimmerID "
                          "LEFT JOIN Buchungsstatus bu "
                          "ON zi.BuchungsstatusID = bu.BuchungsstatusID "
                          "LEFT JOIN Zimmerzusatz zu "
                          "ON be.ZimmerzusatzID = zu.ZimmerzusatzID;");
            queryStatus = query.exec();
            qDebug() << "Abfrage ohne Zimmer und ohne Ausstattung erfolgreich: " << queryStatus;
        }else {
            query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, bu.Status, zu.Zimmerzusatz "
                          "FROM Zimmerbestand zi "
                          "LEFT JOIN BestandZusatzliste be "
                          "ON zi.BestandID = be.BestandID "
                          "LEFT JOIN Zimmer zim "
                          "ON zi.ZimmerID = zim.ZimmerID "
                          "LEFT JOIN Buchungsstatus bu "
                          "ON zi.BuchungsstatusID = bu.BuchungsstatusID "
                          "LEFT JOIN Zimmerzusatz zu "
                          "ON be.ZimmerzusatzID = zu.ZimmerzusatzID "
                          "WHERE :sql;");
            query.bindValue(":sql", sql);
            queryStatus = query.exec();
            if(einzelbett && doppelbett) {
                 qDebug() << "Abfrage mit allen Zimmertypen und mit Ausstattung erfolgreich: " << queryStatus;
            }else if((einzelbett && !doppelbett) || (!einzelbett && doppelbett)) {
                qDebug() << "Abfrage mit Ausstattung erfolgreich: " << queryStatus;
            }else {
                qDebug() << "Abfrage ohne Zimmertypen mit Ausstattung erfolgreich: " << queryStatus;
            }
        }

    }
}

