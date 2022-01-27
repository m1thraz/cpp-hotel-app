#include "hoteldatabaseview.h"
#include "ui_hoteldatabaseview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <iostream>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"

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
            qDebug() << "Ungültiges Zimmernummerformat";
            errormessage error;
            error.changeTextZimmernummerWrong();
            error.setModal(true);
            error.exec();

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
                    qDebug() << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!";
                    errormessage error;
                    error.changeTextZimmertypError();
                    error.setModal(true);
                    error.exec();
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

                if(!veraenderung && !doppelbett && !einzelbett) {
                    qDebug() << "Ausstattungswünsche fehlen";
                    errormessage error;
                    error.changeTextMissingModifications();
                    error.setModal(true);
                    error.exec();
                }else if(veraenderung && queryStatus) {
                    infomessage info;
                    info.changeTextModifiziert();
                    info.setModal(true);
                    info.exec();
                }else if(veraenderung && !queryStatus) {
                    errormessage error;
                    error.changeTextUpdateError();
                    error.setModal(true);
                    error.exec();
                }
            }
            if(!queryStatus) {
                errormessage error;
                error.changeTextDBRequestError();
                error.setModal(true);
                error.exec();

            }else if(queryStatus && !exists) {
                qDebug() << "Zimmer existiert nicht";
                errormessage error;
                error.changeTextZimmernummerError();
                error.setModal(true);
                error.exec();
            }
        }
    }

    // Entweder nur ZimmerID oder Preis angegeben - Führt zu Error
    if((tempZimmerID.isEmpty() && !tempPreis.isEmpty()) ||
             tempPreis.isEmpty() && !tempZimmerID.isEmpty()) {
        qDebug() << "ZimmerID oder Preis fehlt";
        errormessage error;
        error.changeTextPreisError();
        error.setModal(true);
        error.exec();

    // ZimmerID und Preis angegeben
    }else if(!tempZimmerID.isEmpty() && !tempPreis.isEmpty()) {
        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch matchZimmerID = re.match(tempZimmerID);
        QRegularExpressionMatch matchPreis = re.match(tempPreis);

        if(!matchPreis.hasMatch()) {
            qDebug() << "Ungültiges Preisformat";
            errormessage error;
            error.changeTextPreisWrong();
            error.setModal(true);
            error.exec();
        }else if(!matchZimmerID.hasMatch()) {
            qDebug() << "Ungültiges ZimmerID-Format";
            errormessage error;
            error.changeTextZimmerIDWrong();
            error.setModal(true);
            error.exec();

        } else {
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
                if(queryStatus) {
                    infomessage info;
                    info.changeTextModifiziert();
                    info.setModal(true);
                    info.exec();
                }else {
                    errormessage error;
                    error.changeTextUpdateError();
                    error.setModal(true);
                    error.exec();
                }
            }
            if(!queryStatus) {
                errormessage error;
                error.changeTextDBRequestError();
                error.setModal(true);
                error.exec();
            }else if(queryStatus && !exists) {
                qDebug() << "ZimmerID existiert nicht!";
                errormessage error;
                error.changeTextZimmerIDError();
                error.setModal(true);
                error.exec();
            }
        }
    }
    // Alle Eingabefelder (nicht Checkbox) sind leer. Zimmernummer muss nochmal überprüft werden
    else if(tempZimmernummer.isEmpty()){
        qDebug() << "Textfelder sind leer";
        errormessage error;
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
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
            qDebug() << "Ungültiges Zimmernummerformat";
            errormessage error;
            error.changeTextZimmernummerWrong();
            error.setModal(true);
            error.exec();

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
                 qDebug() << "Zimmer existiert bereits!";
                 errormessage error;
                 error.changeTextZimmernummerExists();
                 error.setModal(true);
                 error.exec();
                 exists = true;
            }
            if(!queryStatus) {
                errormessage error;
                error.changeTextDBRequestError();
                error.setModal(true);
                error.exec();
            }else if(queryStatus && !exists) {
                bool doppelbett = ui->checkBoxNeuDoppelbett->isChecked();
                bool einzelbett = ui->checkBoxNeuEinzelbett->isChecked();
                bool aussicht = ui->checkBoxNeuAussicht->isChecked();
                bool fahrstuhl = ui->checkBoxNeuFahrstuhl->isChecked();
                bool sofa = ui->checkBoxNeuSofa->isChecked();
                bool ausstattung = false;
                int bestandID;

                if (doppelbett && einzelbett){
                    ausstattung = true;
                    qDebug() << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!";
                    errormessage error;
                    error.changeTextZimmertypError();
                    error.setModal(true);
                    error.exec();
                }else if(!doppelbett && !einzelbett) {
                    ausstattung = true;
                    qDebug() << "Bett muss Einzelbett oder Doppelbett sein!";
                    errormessage error;
                    error.changeTextZimmertypNotSelected();
                    error.setModal(true);
                    error.exec();
                }else {
                    if(doppelbett && !einzelbett) {
                        // Erstellen eines neuen Zimmers
                       ausstattung = true;
                        query.prepare("INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID)"
                                "VALUES (:zimmerbestand_zimmernummer, 2);");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        queryStatus = query.exec();
                        qDebug() << "Erstellen eines Zimmers mit Doppelbett erfolgreich: " << queryStatus;
                        if(!queryStatus) {
                            errormessage error;
                            error.changeTextDataCreationError();
                            error.setModal(true);
                            error.exec();
                        }else {
                            //Abfrage der neuen BestandID
                            query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                            bool queryStatus = query.exec();
                            qDebug() << "Abfrage erfolgreich: " << queryStatus;
                            while(query.next()) {
                                bestandID = std::stoi(query.value("BestandID").toString().toStdString());
                            }
                            if(!queryStatus) {
                                errormessage error;
                                error.changeTextDBRequestError();
                                error.setModal(true);
                                error.exec();
                            }
                        }
                    }else if(einzelbett && !doppelbett) {
                        // Erstellen eines neuen Zimmers
                        ausstattung = true;
                        query.prepare("INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID)"
                                "VALUES (:zimmerbestand_zimmernummer, 1);");
                        query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                        queryStatus = query.exec();
                        qDebug() << "Erstellen eines Zimmers mit Einzelbett erfolgreich: " << queryStatus;
                        if(!queryStatus) {
                            errormessage error;
                            error.changeTextDataCreationError();
                            error.setModal(true);
                            error.exec();
                        }else {
                            //Abfrage der neuen BestandID
                            query.prepare("SELECT BestandID FROM Zimmerbestand WHERE Zimmernummer = :zimmerbestand_zimmernummer;");
                            query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                            bool queryStatus = query.exec();
                            qDebug() << "Abfrage erfolgreich: " << queryStatus;
                            while(query.next()) {
                                bestandID = std::stoi(query.value("BestandID").toString().toStdString());
                            }
                            if(!queryStatus) {
                                errormessage error;
                                error.changeTextDBRequestError();
                                error.setModal(true);
                                error.exec();
                            }
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
                        qDebug() << "Ausstattungswünsche fehlen";
                        errormessage error;
                        error.changeTextMissingModifications();
                        error.setModal(true);
                        error.exec();
                    }else if(ausstattung && queryStatus) {
                        infomessage info;
                        info.changeTextNeu();
                        info.setModal(true);
                        info.exec();
                    }else if(ausstattung && !queryStatus) {
                        errormessage error;
                        error.changeTextDataCreationError();
                        error.setModal(true);
                        error.exec();
                    }
                }
            }
        }
    // Eingabefeld für Zimmernummer (nicht Checkbox) ist leer
    }else {
        qDebug() << "Textfeld ist leer";
        errormessage error;
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
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

    // MUSS BEARBEITET WERDEN MIT DER NEUEN DATUMSLOGIK
//    if(verfuegbar) {
//        ausstattung = true;
//        sql += " AND bu.BuchungsstatusID = 1";
//    }

    QSqlQuery query;
    bool queryStatus;

    // Zimmernummer angegeben
    if(!tempZimmernummer.isEmpty()) {

        QRegularExpression re("^[0-9]+$");
        QRegularExpressionMatch match = re.match(tempZimmernummer);
        if(!match.hasMatch()) {
            qDebug() << "Ungültiges Zimmernummerformat";
            errormessage error;
            error.changeTextZimmernummerWrong();
            error.setModal(true);
            error.exec();

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
            if(!queryStatus) {
                errormessage error;
                error.changeTextDBRequestError();
                error.setModal(true);
                error.exec();
            }
            if(queryStatus && !exists) {
                qDebug() << "Ausgewähltes Zimmer existiert nicht";
                errormessage error;
                error.changeTextZimmernummerError();
                error.setModal(true);
                error.exec();

            }else if(!ausstattung) {
                query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, zu.Zimmerzusatz "
                              "FROM Zimmerbestand zi "
                              "LEFT JOIN BestandZusatzliste be "
                              "ON zi.BestandID = be.BestandID "
                              "LEFT JOIN Zimmer zim "
                              "ON zi.ZimmerID = zim.ZimmerID "
                              "LEFT JOIN Zimmerzusatz zu "
                              "ON be.ZimmerzusatzID = zu.ZimmerzusatzID "
                              "WHERE zi.Zimmernummer = :zimmerbestand_zimmernummer;");
                query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                queryStatus = query.exec();
                qDebug() << "Abfrage mit Zimmer erfolgreich: " << queryStatus;
            }else {
                query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, zu.Zimmerzusatz "
                              "FROM Zimmerbestand zi "
                              "LEFT JOIN BestandZusatzliste be "
                              "ON zi.BestandID = be.BestandID "
                              "LEFT JOIN Zimmer zim "
                              "ON zi.ZimmerID = zim.ZimmerID "
                              "LEFT JOIN Zimmerzusatz zu "
                              "ON be.ZimmerzusatzID = zu.ZimmerzusatzID "
                              "WHERE zi.Zimmernummer = :zimmerbestand_zimmernummer AND :sql;");
                query.bindValue(":zimmerbestand_zimmernummer", zimmernummer);
                query.bindValue(":sql", sql);
                queryStatus = query.exec();
                qDebug() << "Abfrage mit Zimmer und Ausstattung erfolgreich: " << queryStatus;
            }
            if(!queryStatus) {
                errormessage error;
                error.changeTextDBRequestError();
                error.setModal(true);
                error.exec();
            }else{
                //HIER EIN NEUES GUI FENSTER MIT DER ANZUZEIGENDEN ABFRAGE ÖFFNEN!!!
            }
        }
    }else {
        if(!ausstattung) {
            query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, zu.Zimmerzusatz "
                          "FROM Zimmerbestand zi "
                          "LEFT JOIN BestandZusatzliste be "
                          "ON zi.BestandID = be.BestandID "
                          "LEFT JOIN Zimmer zim "
                          "ON zi.ZimmerID = zim.ZimmerID "
                          "LEFT JOIN Zimmerzusatz zu "
                          "ON be.ZimmerzusatzID = zu.ZimmerzusatzID;");
            queryStatus = query.exec();
            qDebug() << "Abfrage ohne Zimmer und ohne Ausstattung erfolgreich: " << queryStatus;
        }else {
            query.prepare("SELECT zi.BestandID, zi.Zimmernummer, zim.Zimmertyp, zim.Zimmerkosten, zu.Zimmerzusatz "
                          "FROM Zimmerbestand zi "
                          "LEFT JOIN BestandZusatzliste be "
                          "ON zi.BestandID = be.BestandID "
                          "LEFT JOIN Zimmer zim "
                          "ON zi.ZimmerID = zim.ZimmerID "
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
        if(!queryStatus) {
            errormessage error;
            error.changeTextDBRequestError();
            error.setModal(true);
            error.exec();
        }else{
            //HIER EIN NEUES GUI FENSTER MIT DER ANZUZEIGENDEN ABFRAGE ÖFFNEN!!!
        }
    }
}

