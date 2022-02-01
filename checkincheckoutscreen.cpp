#include "checkincheckoutscreen.h"
#include "ui_checkincheckoutscreen.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include "verifier.h"

checkInCheckOutScreen::checkInCheckOutScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkInCheckOutScreen)
{
    ui->setupUi(this);
}

checkInCheckOutScreen::~checkInCheckOutScreen()
{
    delete ui;
}



void checkInCheckOutScreen::on_checkInButton_clicked() {
    if(!lineEditVerification(1)) {
        return;
    }

    errormessage error;
    if(!this->getKundenID() || !this->getBuchungsID()) {
        error.changeTextMissingInputText();
        error.setModal(true);
        error.exec();
        return;
    }

    verifier verify;

    if(!verify.verifyKundenIDExists(this->getKundenID())) {
        return;
    }

    if(!verify.verifyBuchungExists(this->getBuchungsID(), this->getKundenID())) {
        return;
    }

    // Einchecken wird eingeleitet
    QSqlQuery query;

    query.prepare("UPDATE Zimmerbuchungsliste SET BuchungsstatusID = 2 "
          "WHERE BuchungsID = :buchung_buchungsID;");
    query.bindValue(":buchung_buchungsID", this->getBuchungsID());
    bool queryStatus = query.exec();
    qDebug() << "Einchecken erfolgreich: " << queryStatus;

    if(!queryStatus) {
        error.changeTextUpdateError();
        error.setModal(true);
        error.exec();
    }else {
        infomessage info;
        info.changeTextModifiziert();
        info.setModal(true);
        info.exec();
    }
}

void checkInCheckOutScreen::on_checkOutBtn_clicked() {
    if(!lineEditVerification(2)) {
        return;
    }
}

bool checkInCheckOutScreen::lineEditVerification(const int buttontyp) {
    QString tempKundenID;
    QString tempBuchungsID;

    switch(buttontyp) {
    case 1:
        tempKundenID = ui->lineEditInKundenID->text();
        tempBuchungsID = ui->lineEditInBuchungsID->text();
        break;
    case 2:
        tempKundenID = ui->lineEditOutKundenID->text();
        tempBuchungsID = ui->lineEditOutBuchungsID->text();
        break;
    default:
        qDebug() << "Fehler beim Verifikationsprozess der lineEdits aufgetreten";
        return false;
    }

    // Anfängliche Überprüfung aller textuellen Eingaben
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

    match = numbers.match(tempBuchungsID);
    if(!tempBuchungsID.isEmpty() && match.hasMatch()) {
        this->setBuchungsID(std::stoi(tempBuchungsID.toStdString()));
    }else if(!tempBuchungsID.isEmpty() && !match.hasMatch()) {
        qDebug() << "Ungültiges BuchungsID-Format";
        error.changeTextBuchungsIDError();
        error.setModal(true);
        error.exec();
        return false;
    }else {
        this->setBuchungsID(0);
    }

    //Entweder alle Felder leer, oder Überprüfungen mit positivem Ergebnis
    return true;
}

void checkInCheckOutScreen::setKundenID(int kundenID) {
    this->kundenID = kundenID;
}

int checkInCheckOutScreen::getKundenID() {
    return this->kundenID;
}

void checkInCheckOutScreen::setBuchungsID(int buchungsID) {
    this->buchungsID = buchungsID;
}

int checkInCheckOutScreen::getBuchungsID() {
    return this->buchungsID;
}
