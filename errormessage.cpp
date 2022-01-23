#include "errormessage.h"
#include "ui_errormessage.h"

errormessage::errormessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errormessage)
{
    ui->setupUi(this);
}

errormessage::~errormessage()
{
    delete ui;
}

void errormessage::on_closeWindowButton_clicked()
{
    this->hide();
}

void errormessage::changeTextIDWrong() {
    ui->labelErrorMessage->setText("<p>Ihre Login-ID ist fehlerhaft.</p><p>Bitte geben Sie Ihre Anmeldedaten erneut ein.</p>");
}

void errormessage::changeTextLoginError() {
    ui->labelErrorMessage->setText("<p>Ihre Login-ID oder Ihr eingegebenes Passwort sind<br>falsch.</p><p>Bitte versuchen Sie es erneut.</p>");
}
