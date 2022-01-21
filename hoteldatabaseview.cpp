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
            query.prepare("SELECT 1 FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
            bool queryStatus = query.exec();
            qDebug() << "Abfrage erfolgreich: " << queryStatus;

            //Wird nur ausgeführt, wenn es das Zimmer tatsächlich gibt, sonst Fehlermeldung
            while(query.next()) {
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
                   // query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                    queryStatus = query.exec();
                    qDebug() << "Update Doppelbett erfolgreich: " << queryStatus;
                }else if(einzelbett && !doppelbett) {
                    veraenderung = true;
                    query.prepare("UPDATE Zimmerzusatz SET ZimmerID = 1 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    queryStatus = query.exec();
                    qDebug() << "Update Einzelbett erfolgreich: " << queryStatus;
                }
                else if (doppelbett && einzelbett){
                    std::cout << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!" << std::endl;
                }

                if(aussicht) {
                    veraenderung = true;
                    query.prepare("UPDATE BestandZusatzliste SET ZimmerzusatzID = 1 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    queryStatus = query.exec();
                    qDebug() << "Update Aussicht erfolgreich: " << queryStatus;
                }

                if(fahrstuhl) {
                    veraenderung = true;
                    query.prepare("UPDATE BestandZusatzliste SET ZimmerzusatzID = 2 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    queryStatus = query.exec();
                    qDebug() << "Update Fahrstuhl erfolgreich: " << queryStatus;
                }

                if(sofa) {
                    veraenderung = true;
                    query.prepare("UPDATE BestandZusatzliste SET ZimmerzusatzID = 3 WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                    queryStatus = query.exec();
                    qDebug() << "Update Sofa erfolgreich: " << queryStatus;
                }

                if(!veraenderung) {
                    std::cout << "Bitte geben Sie neben der Zimmernummer die Veränderungswünsche an!" << std::endl;
                }
            }
        }
    // Entweder nur ZimmerID oder Preis angegeben - Führt zu Error
    }else if((tempZimmerID.isEmpty() && !tempPreis.isEmpty()) ||
             tempPreis.isEmpty() && !tempZimmerID.isEmpty()) {
        /*
         * GUI ERROR MESSAGE - Für das Anpassen eines Zimmerpreises, muss eine ZimmerID eingegeben werden.
         * Das Anpassen einer ZimmerID ist nicht möglich!
         */
        std::cout << "test" << std::endl;
    }else if(!tempZimmerID.isEmpty() && !tempPreis.isEmpty()) {
         std::cout << "Preisanpassung" << std::endl;
    }
    else {
        std::cout << "nix ausgewählt" << std::endl;
    }
}
