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

        QRegularExpression re("[0-9]+");
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
                bool fahrstuhl = ui->checkBoxModAussicht->isChecked();
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
        QRegularExpression re("[0-9]+");
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
