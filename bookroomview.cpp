#include "bookroomview.h"
#include "ui_bookroomview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"

bookroomview::bookroomview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookroomview)
{
    ui->setupUi(this);
}

bookroomview::~bookroomview()
{
    delete ui;
}

void bookroomview::on_bookRoomButton_clicked() {
    if(lineEditVerification(1)) {
        qDebug() << QString::fromStdString(this->getAnreiseDatum());
    }
}

void bookroomview::on_bookExtrasButton_clicked() {
    if(lineEditVerification(2)) {

    }
}

bool bookroomview::lineEditVerification(const int buttontyp) {
    QString tempKundenID;
    QString tempMitarbeiterID;
    QString tempBestandID;
    QString tempAnreiseDatum;
    QString tempAbreiseDatum;

    switch(buttontyp) {
    case 1:
        tempKundenID = ui->LineEditNeuKundenID->text();
        tempMitarbeiterID = ui->LineEditNeuMitarbeiterID->text();
        tempBestandID = ui->LineEditNeuZimmerID->text();
        tempAnreiseDatum = ui->dateEditNeuAnreiseDatum->text();
        tempAbreiseDatum = ui->dateEditNeuAbreiseDatum->text();
        break;
    case 2:
        tempKundenID = ui->lineEditSonderKundenID->text();
        tempMitarbeiterID = ui->lineEditSonderMitarbeiterID->text();
        break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }

    //Anfängliche Überprüfung aller textuellen Eingaben
    QRegularExpression numbers("[0-9]+$");
    QRegularExpressionMatch match;
    errormessage error;

    match = numbers.match(tempKundenID);
    if(!tempKundenID.isEmpty() && match.hasMatch()) {
        this->setKundenID(std::stoi(tempKundenID.toStdString()));
    }else if(!tempKundenID.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges KundenID-Format";
        error.changeTextKundenIDWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setKundenID(0);
    }

    match = numbers.match(tempMitarbeiterID);
    if(!tempMitarbeiterID.isEmpty() && match.hasMatch()) {
        this->setMitarbeiterID(std::stoi(tempMitarbeiterID.toStdString()));
    }else if(!tempMitarbeiterID.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges MitarbeiterID-Format";
        error.changeTextMitarbeiterIDWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setMitarbeiterID(0);
    }

    match = numbers.match(tempBestandID);
    if(!tempBestandID.isEmpty() && match.hasMatch()) {
        this->setBestandID(std::stoi(tempBestandID.toStdString()));
    }else if(!tempBestandID.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges BestandID-Format";
        error.changeTextBestandIDWrong();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setBestandID(0);
    }

    // Kein Match für Anreise und Abreise, da Datumsfeld
    if(!tempAnreiseDatum.isEmpty()) {
        this->setAnreiseDatum(tempAnreiseDatum.toStdString());
    }
    if(!tempAbreiseDatum.isEmpty()) {
        this->setAbreiseDatum(tempAbreiseDatum.toStdString());
    }

    //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
    return true;
}

void bookroomview::setKundenID(int kundenID) {
    this->kundenID = kundenID;
}

int bookroomview::getKundenID() {
    return this->kundenID;
}

void bookroomview::setMitarbeiterID(int mitarbeiterID) {
    this->mitarbeiterID = mitarbeiterID;
}

int bookroomview::getMitarbeiterID() {
    return this->mitarbeiterID;
}

void bookroomview::setBestandID(int bestandID) {
    this->bestandID = bestandID;
}

int bookroomview::getBestandID() {
    return this->bestandID;
}

void bookroomview::setAnreiseDatum(std::string anreisedatum) {
    this->anreisedatum = anreisedatum;
}

std::string bookroomview::getAnreiseDatum() {
    return this->anreisedatum;
}

void bookroomview::setAbreiseDatum(std::string abreisedatum) {
    this->abreisedatum = abreisedatum;
}

std::string bookroomview::getAbreiseDatum() {
    return this->abreisedatum;
}
