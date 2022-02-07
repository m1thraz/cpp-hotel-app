#include "hoteldatabaseview.h"
#include "ui_hoteldatabaseview.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"
#include "database.h"

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
    if(!lineEditVerification(2)) {
        return;
    }

    errormessage error;
    verifier verify;

    // Alle lineEdit-Eingabefelder (nicht Checkbox) sind leer.
    if(this->getZimmerID() == 0 && this->getPreis() == 0 && this->getZimmernummer() == 0) {
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
        return;
    }

    // Entweder nur ZimmerID oder Preis angegeben
    if((this->getZimmerID() == 0 || this->getPreis() == 0) && this->getZimmernummer() == 0) {
        error.changeTextPreisError();
        error.setModal(true);
        error.exec();
        return;

        // ZimmerID und Preis angegeben
    }else if(this->getZimmerID() != 0 && this->getPreis() != 0 && this->getZimmernummer() == 0) {
        if(!verify.verifyZimmerIDExists(this->getZimmerID())) {
            return;
        }
        // Preisanpassung wird eingeleitet
        modPreis();

        // Nur Zimmernummer angegeben
    }else if(this->getZimmerID() == 0 && this->getPreis() == 0 && this->getZimmernummer() != 0) {
        if(!verify.verifyZimmernummerExists(this->getZimmernummer())) {
            return;
        }
        // Zimmer Modifizierung wird eingeleitet
        modZimmer();

        //Alles angegeben
    }else {
        if(!verify.verifyZimmerIDExists(this->getZimmerID())) {
            return;
        }
        if(!verify.verifyZimmernummerExists(this->getZimmernummer())) {
            return;
        }
        // Preisanpassung wird eingeleitet
        modPreis();
        // Zimmer Modifizierung wird eingeleitet
        modZimmer();
    }
}

// Modifizierung der Zimmerausstattung
void hotelDatabaseView::modZimmer() {
    Database db;
    QSqlQuery query;
    errormessage error;
    int bestandID = db.getBestandID(this->getZimmernummer());

    // QCheckBox Felder werden ausgewertet
    bool doppelbett = ui->checkBoxModDoppelbett->isChecked();
    bool einzelbett = ui->checkBoxModEinzelbett->isChecked();
    bool aussicht = ui->checkBoxModAussicht->isChecked();
    bool fahrstuhl = ui->checkBoxModFahrstuhl->isChecked();
    bool sofa = ui->checkBoxModSofa->isChecked();
    bool veraenderung = false;
    bool queryStatus;
    int zimmertyp;

    if (doppelbett && einzelbett){
        qDebug() << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!";
        error.changeTextZimmertypError();
        error.setModal(true);
        error.exec();
        return;
    }else if(doppelbett) {
        veraenderung = true;
        zimmertyp = 2;
    }else if(einzelbett) {
        veraenderung = true;
        zimmertyp = 1;
    }

    if(veraenderung) {
        query.prepare("UPDATE Zimmerbestand SET ZimmerID = :zimmerID WHERE BestandID = :zimmerbestand_bestandID;");
        query.bindValue(":zimmerbestand_bestandID", bestandID);
        query.bindValue(":zimmerID", zimmertyp);
        queryStatus = query.exec();
        qDebug() << "Update ZimmerID erfolgreich: " << queryStatus;

        // Beim Updaten des Zimmertyps ist ein Fehler seitens der DB aufgetreten.
        if(!queryStatus) {
            error.changeTextDBRequestError();
            error.setModal(true);
            error.exec();
            return;
        }
    }

    int zimmerzusatzID[3] = {1, 2, 3};
    bool ausstattung[3] = {aussicht, fahrstuhl, sofa};

    for(int i = 0; i < 3; i++) {
        if(ausstattung[i]) {
            veraenderung = true;
            query.prepare("INSERT OR IGNORE INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                          "VALUES (:zimmerbestand_bestandID, :zimmerbestand_zimmerzusatzID);");
            query.bindValue(":zimmerbestand_bestandID", bestandID);
            query.bindValue(":zimmerbestand_zimmerzusatzID", zimmerzusatzID[i]);
            queryStatus = query.exec();
            qDebug() << "Hinzufügen der Ausstattung erfolgreich: " << queryStatus;

            if(!queryStatus) {
                errormessage error;
                error.changeTextDataCreationError();
                error.setModal(true);
                error.exec();
            }
        }
    }

    if(!veraenderung) {
        qDebug() << "Ausstattungswünsche fehlen";
        error.changeTextMissingModifications();
        error.setModal(true);
        error.exec();
    }else if(queryStatus) {
        infomessage info;
        info.changeTextModifiziert();
        info.setModal(true);
        info.exec();
    }
}

// Preisanpassung des Zimmertyps
void hotelDatabaseView::modPreis() {
    QSqlQuery query;
    query.prepare("UPDATE Zimmer SET Zimmerkosten = :zimmer_preis WHERE ZimmerID = :zimmer_zimmerID;");
    query.bindValue(":zimmer_preis", this->getPreis());
    query.bindValue(":zimmer_zimmerID", this->getZimmerID());
    bool queryStatus = query.exec();
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

void hotelDatabaseView::on_eintragenButton_clicked(){
    if(!lineEditVerification(3)) {
        return;
    }

    errormessage error;
    verifier verify;

    if(this->getZimmernummer() == 0) {
        // LineEdit-Eingabefeld für Zimmernummer (nicht Checkbox) ist leer
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
        return;
    }

    // Zimmer existiert bereits
    if(!verify.verifyZimmernummerDoesntExist(this->getZimmernummer())) {
        return;
    }

    // QCheckBox Felder werden ausgewertet
    bool doppelbett = ui->checkBoxNeuDoppelbett->isChecked();
    bool einzelbett = ui->checkBoxNeuEinzelbett->isChecked();
    bool aussicht = ui->checkBoxNeuAussicht->isChecked();
    bool fahrstuhl = ui->checkBoxNeuFahrstuhl->isChecked();
    bool sofa = ui->checkBoxNeuSofa->isChecked();
    int zimmertyp;

    if (doppelbett && einzelbett){
        qDebug() << "Bett kann nicht Einzelbett und Doppelbett gleichzeitig sein!";
        error.changeTextZimmertypError();
        error.setModal(true);
        error.exec();
        return;
    }else if(!doppelbett && !einzelbett) {
        qDebug() << "Bett muss Einzelbett oder Doppelbett sein!";
        error.changeTextZimmertypNotSelected();
        error.setModal(true);
        error.exec();
        return;
    }else if(doppelbett) {
        zimmertyp = 2;
    }else if(einzelbett) {
        zimmertyp = 1;
    }

    // Erstellen eines neuen Zimmers
    QSqlQuery query;
    bool queryStatus;
    query.prepare("INSERT INTO Zimmerbestand (Zimmernummer, ZimmerID)"
                  "VALUES (:zimmerbestand_zimmernummer, :zimmerbestand_zimmerID);");
    query.bindValue(":zimmerbestand_zimmernummer", this->getZimmernummer());
    query.bindValue(":zimmerbestand_zimmerID", zimmertyp);
    queryStatus = query.exec();
    qDebug() << "Erstellen eines Zimmers erfolgreich: " << queryStatus;
    if(!queryStatus) {
        errormessage error;
        error.changeTextDataCreationError();
        error.setModal(true);
        error.exec();
        return;
    }

    //Abfrage der neuen BestandID
    Database db;
    int bestandID = db.getBestandID(this->getZimmernummer());

    int zimmerzusatzID[3] = {1, 2, 3};
    bool ausstattung[3] = {aussicht, fahrstuhl, sofa};

    for(int i = 0; i < 3; i++) {
        if(ausstattung[i]) {
            query.prepare("INSERT INTO BestandZusatzliste (BestandID, ZimmerzusatzID)"
                          "VALUES (:zimmerbestand_bestandID, :zimmerbestand_zimmerzusatzID);");
            query.bindValue(":zimmerbestand_bestandID", bestandID);
            query.bindValue(":zimmerbestand_zimmerzusatzID", zimmerzusatzID[i]);
            queryStatus = query.exec();
            qDebug() << "Hinzufügen der Ausstattung erfolgreich: " << queryStatus;

            if(!queryStatus) {
                errormessage error;
                error.changeTextDataCreationError();
                error.setModal(true);
                error.exec();
            }
        }
    }

    if(queryStatus) {
        infomessage info;
        info.changeTextNeu();
        info.setModal(true);
        info.exec();
    }
}

void hotelDatabaseView::on_suchenButton_clicked() {
    if(!lineEditVerification(1)) {
        return;
    }

    verifier verify;
    bool zimmernummer;
    QString whereSqlZimmer = "Zimmernummer = ";

    if(this->getZimmernummer() == 0) {
        zimmernummer = false;
    }else if(this->getZimmernummer() != 0 && !verify.verifyZimmernummerExists(this->getZimmernummer())) {
        return;
    }else{
        zimmernummer = true;
        whereSqlZimmer += QString::fromStdString(std::to_string(this->getZimmernummer()));
    }

    // QCheckBox Felder werden ausgewertet
    bool doppelbett = ui->checkBoxAbfrDoppelbett->isChecked();
    bool einzelbett = ui->checkBoxAbfrEinzelbett->isChecked();
    bool aussicht = ui->checkBoxAbfrAussicht->isChecked();
    bool fahrstuhl = ui->checkBoxAbfrFahrstuhl->isChecked();
    bool sofa = ui->checkBoxAbfrSofa->isChecked();
    bool nurVerfuegbare = ui->radioButtonAbfrVerfuegbar->isChecked();

    bool whereBedingungExtras = false;
    bool whereBedingungVerfuegbar = false;
    QString whereSqlExtras;
    QString whereSqlVerfuegbar;

    if(doppelbett && !einzelbett) {
        whereBedingungExtras = true;
        whereSqlExtras = "Zimmertyp = 'Doppelzimmer'";
    }else if(einzelbett && !doppelbett) {
        whereBedingungExtras = true;
        whereSqlExtras = "Zimmertyp = 'Einzelzimmer'";
    }

    if(whereBedingungExtras && (aussicht || fahrstuhl || sofa)) {
        whereSqlExtras += " AND ";
    }else if(aussicht || fahrstuhl || sofa) {
        whereBedingungExtras = true;
    }

    if(!aussicht && !fahrstuhl && sofa) {
        whereSqlExtras += "Sofa = 'true'";

    }else if(!aussicht && fahrstuhl && !sofa) {
        whereSqlExtras += "Fahrstuhlnähe = 'true'";
    }

    else if(!aussicht && fahrstuhl && sofa) {
        whereSqlExtras += "Fahrstuhlnähe = 'true' AND Sofa = 'true'";
    }

    else if(aussicht && !fahrstuhl && !sofa) {
        whereSqlExtras += "Aussicht = 'true'";
    }

    else if(aussicht && !fahrstuhl && sofa) {
        whereSqlExtras += "Aussicht = 'true' AND Sofa = 'true'";
    }

    else if(aussicht && fahrstuhl && !sofa) {
        whereSqlExtras += "Aussicht = 'true' AND Fahrstuhlnähe = 'true'";
    }

    else if(aussicht && fahrstuhl && sofa) {
        whereSqlExtras += "Aussicht = 'true' AND Fahrstuhlnähe = 'true' AND Sofa = 'true'";
    }

    if(nurVerfuegbare) {
        whereBedingungVerfuegbar = true;
        std::string tempwhereSqlVerfuegbar = " LEFT JOIN Zimmerbuchungsliste bu ON hot.BestandID = bu.BestandID "
                                             "WHERE (bu.Anreisedatum NOT BETWEEN '" + this->getAnreiseDatum() +
                "' AND '" + this->getAbreiseDatum() +
                "' OR bu.Abreisedatum NOT BETWEEN '" + this->getAnreiseDatum() +
                "' AND '" + this->getAbreiseDatum() +
                "') OR (bu.Anreisedatum < '" + this->getAnreiseDatum() +
                "' AND bu.Abreisedatum > '" + this->getAbreiseDatum() +
                "') OR ifnull(bu.Anreisedatum,'') = ''";
        whereSqlVerfuegbar += QString::fromStdString(tempwhereSqlVerfuegbar);
    }

    QSqlQuery query;
    bool queryStatus;
    QString select = "SELECT hot.BestandID, hot.Zimmernummer, hot.Zimmertyp, hot. Zimmerkosten, "
                     "hot.Aussicht, hot.Fahrstuhlnähe, hot.Sofa "
                     "FROM Hotelzimmer hot";
    //Alle Zimmer
    if(!whereBedingungExtras && !zimmernummer && !whereBedingungVerfuegbar) {
        query.prepare(select + ";");
        queryStatus = query.exec();
        qDebug() << "Zimmersuche erfolgreich: " << queryStatus;

    }else if(!whereBedingungExtras && zimmernummer && !whereBedingungVerfuegbar) {
        query.prepare(select + " WHERE " + whereSqlZimmer + ";");
        queryStatus = query.exec();
        qDebug() << "Zimmersuche mit Zimmer erfolgreich: " << queryStatus;

    }else if(whereBedingungExtras && zimmernummer && !whereBedingungVerfuegbar) {
        query.prepare(select + " WHERE " + whereSqlZimmer + " AND " + whereSqlExtras + ";");
        queryStatus = query.exec();
        qDebug() << "Zimmersuche mit Zimmer und Extras erfolgreich: " << queryStatus;

    }else if(whereBedingungExtras && !zimmernummer && !whereBedingungVerfuegbar){
        query.prepare(select + " WHERE " + whereSqlExtras + ";");
        queryStatus = query.exec();
        qDebug() << "Zimmersuche mit Extras erfolgreich: " << queryStatus;

    }else if(!whereBedingungExtras && !zimmernummer && whereBedingungVerfuegbar) {
        query.prepare(select + whereSqlVerfuegbar + ";");
        queryStatus = query.exec();
        qDebug() << "Nur verfügbare Zimmersuche erfolgreich: " << queryStatus;

    }else if(!whereBedingungExtras && zimmernummer && whereBedingungVerfuegbar) {
        query.prepare(select + whereSqlVerfuegbar + " AND " + whereSqlZimmer + ";");
        queryStatus = query.exec();
        qDebug() << "Nur verfügbare Zimmersuche mit Zimmer erfolgreich: " << queryStatus;

    }else if(whereBedingungExtras && !zimmernummer && whereBedingungVerfuegbar) {
        query.prepare(select + whereSqlVerfuegbar + " AND " + whereSqlExtras + ";");
        queryStatus = query.exec();
        qDebug() << "Nur verfügbare Zimmersuche mit Extras erfolgreich: " << queryStatus;

    }else if(whereBedingungExtras && zimmernummer && whereBedingungVerfuegbar) {
        query.prepare(select + whereSqlVerfuegbar + " AND " + whereSqlZimmer + " AND " + whereSqlExtras + ";");
        queryStatus = query.exec();
        qDebug() << "Nur verfügbare Zimmersuche mit Zimmer und Extras erfolgreich: " << queryStatus;
    }

    if(!queryStatus) {
        errormessage error;
        error.changeTextDBRequestError();
        error.setModal(true);
        error.exec();
        return;
    }

    while(query.next() == true) {
        hoteldata.push_back("BestandID: ");
        hoteldata.push_back(query.value("BestandID").toString().toStdString());
        hoteldata.push_back(", Zimmernummer: ");
        hoteldata.push_back(query.value("Zimmernummer").toString().toStdString());
        hoteldata.push_back(", Zimmertyp: ");
        hoteldata.push_back(query.value("Zimmertyp").toString().toStdString());
        hoteldata.push_back(", Zimmerkosten: ");
        hoteldata.push_back(query.value("Zimmerkosten").toString().toStdString());
        hoteldata.push_back(", Gute Aussicht: ");

        if(query.value("Aussicht") == true) {
            hoteldata.push_back("Gute Aussicht");
        } else {
            hoteldata.push_back("Keine gute Aussicht");
        }

        hoteldata.push_back(", Fahrstuhlnähe: ");

        if(query.value("Fahrstuhlnähe") == true) {
            hoteldata.push_back("In Fahrstuhlnähe");
        } else {
            hoteldata.push_back("Nicht in Fahrstuhlnähe");
        }

        hoteldata.push_back(", Schlafsofa: ");

        if(query.value("Schlafsofa") == true) {
            hoteldata.push_back("Schlafsofa verfügbar");
        } else {
            hoteldata.push_back("Schlafsofa nicht verfügbar");
        }
    }

    std::string checkhotelInput;
    for (auto& s:hoteldata) {
        checkhotelInput+=s;
    }
    qDebug() << "Folgende Daten wurden gespeichert: " << checkhotelInput.c_str();

    //HIER EIN NEUES GUI FENSTER MIT DER ANZUZEIGENDEN ABFRAGE ÖFFNEN!!!

//    hotelsearch = new displayhotelsearch(this);
//    hotelsearch -> show();
}

void hotelDatabaseView::setHoteldata(std::vector<std::string> &) {
    this->hoteldata = hoteldata;
}

void hotelDatabaseView::on_entfernenButton_clicked() {
    if(!lineEditVerification(2)) {
        return;
    }

    errormessage error;
    verifier verify;

    // lineEdit-Eingabefeld (nicht Checkbox) für Zimmernummer ist leer
    if(this->getZimmernummer() == 0) {
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
        return;
    }

    if(!verify.verifyZimmernummerExists(this->getZimmernummer())) {
        return;
    }

    //Das Entfernen der Raumausstattung wird eingeleitet
    removeRoomEquipment();
}

void hotelDatabaseView::removeRoomEquipment() {
    Database db;
    QSqlQuery query;
    errormessage error;
    int bestandID = db.getBestandID(this->getZimmernummer());

    // QCheckBox Felder werden ausgewertet

    bool aussicht = ui->checkBoxModAussicht->isChecked();
    bool fahrstuhl = ui->checkBoxModFahrstuhl->isChecked();
    bool sofa = ui->checkBoxModSofa->isChecked();
    bool queryStatus;

    if(!aussicht && !fahrstuhl && !sofa) {
        qDebug() << "Zu entfernende Sonderausstattung fehlt";
        error.changeTextMissingModifications();
        error.setModal(true);
        error.exec();
        return;
    }

    bool ausstattung[3] = {aussicht, fahrstuhl, sofa};
    for(int i = 0; i < 3; i++) {
        if(ausstattung[i]) {
            query.prepare("DELETE FROM BestandZusatzliste WHERE "
                          "ZimmerzusatzID = :zusatzID AND BestandID = :bestandID;");
            query.bindValue(":zusatzID", QString::fromStdString(std::to_string(i+1)));
            query.bindValue(":bestandID", bestandID);
            queryStatus = query.exec();
            qDebug() << "Entfernen der Ausstattung erfolgreich: " << queryStatus;

            if(!queryStatus) {
                errormessage error;
                error.changeTextUpdateError();
                error.setModal(true);
                error.exec();
                return;
            }
        }
    }

    infomessage info;
    info.changeTextModifiziert();
    info.setModal(true);
    info.exec();
}

bool hotelDatabaseView::lineEditVerification(const int buttontyp) {
    QString tempZimmerID;
    QString tempZimmernummer;
    QString tempPreis;
    QString tempAnreiseDatum;
    QString tempAbreiseDatum;

    switch(buttontyp) {
    case 1:
        tempZimmernummer = ui->lineEditAbfrZimmernummer->text();
        tempAnreiseDatum = ui->dateEditAbfrAnreiseDatum->text();
        tempAbreiseDatum = ui->dateEditAbfrAbreiseDatum->text();
        break;
    case 2:
        tempZimmerID = ui->lineEditModZimmerID->text();
        tempPreis = ui->lineEditModPreis->text();
        tempZimmernummer = ui->lineEditModZimmernummer->text();
        break;
    case 3:
        tempZimmernummer = ui->lineEditNeuZimmernummer->text();
        break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }

    //Anfängliche Überprüfung aller textuellen Eingaben
    QRegularExpression numbers("[0-9]+$");
    QRegularExpressionMatch match;
    errormessage error;

    match = numbers.match(tempZimmerID);
    if(!tempZimmerID.isEmpty() && match.hasMatch()) {
        this->setZimmerID(std::stoi(tempZimmerID.toStdString()));
    }else if(!tempZimmerID.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges ZimmerID-Format";
        error.changeTextZimmerIDWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setZimmerID(0);
    }

    match = numbers.match(tempZimmernummer);
    if(!tempZimmernummer.isEmpty() && match.hasMatch()) {
        this->setZimmernummer(std::stoi(tempZimmernummer.toStdString()));
    }else if(!tempZimmernummer.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Zimmernummer-Format";
        error.changeTextZimmernummerWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setZimmernummer(0);
    }

    match = numbers.match(tempPreis);
    if(!tempPreis.isEmpty() && match.hasMatch()) {
        this->setPreis(std::stoi(tempPreis.toStdString()));
    }else if(!tempPreis.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges Preis-Format";
        error.changeTextPreisWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setPreis(0);
    }

    // Kein Match für Anreise und Abreise, da Datumsfeld
    if(!tempAnreiseDatum.isEmpty()) {
        tempAnreiseDatum.replace(".", "-");
        this->setAnreiseDatum(tempAnreiseDatum.toStdString());
    }
    if(!tempAbreiseDatum.isEmpty()) {
        tempAbreiseDatum.replace(".", "-");
        this->setAbreiseDatum(tempAbreiseDatum.toStdString());
    }

    //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
    return true;
}

void hotelDatabaseView::setZimmerID(int zimmerID) {
    this->zimmerID = zimmerID;
}

int hotelDatabaseView::getZimmerID() {
    return this->zimmerID;
}

void hotelDatabaseView::setZimmernummer(int zimmernummer) {
    this->zimmernummer = zimmernummer;
}

int hotelDatabaseView::getZimmernummer() {
    return this->zimmernummer;
}

void hotelDatabaseView::setPreis(int preis) {
    this->preis = preis;
}

int hotelDatabaseView::getPreis() {
    return this->preis;
}

void hotelDatabaseView::setAnreiseDatum(std::string anreisedatum) {
    this->anreisedatum = anreisedatum;
}

std::string hotelDatabaseView::getAnreiseDatum() {
    return this->anreisedatum;
}

void hotelDatabaseView::setAbreiseDatum(std::string abreisedatum) {
    this->abreisedatum = abreisedatum;
}

std::string hotelDatabaseView::getAbreiseDatum() {
    return this->abreisedatum;
}

void hotelDatabaseView::on_radioButtonAbfrVerfuegbar_toggled(bool checked) {
    ui->dateEditAbfrAnreiseDatum->setEnabled(checked);
    ui->dateEditAbfrAbreiseDatum->setEnabled(checked);
}
