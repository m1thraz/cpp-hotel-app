#include "infologmessage.h"
#include "ui_infologmessage.h"

infologmessage::infologmessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infologmessage)
{
    ui->setupUi(this);
}

infologmessage::~infologmessage()
{
    delete ui;
}

void infologmessage::on_closeWindowButton_clicked()
{
    this->hide();
}

void infologmessage::changeTextIDWrong() {
    ui->labelErrorMessage->setText("<p>Ihre Login-ID ist fehlerhaft.</p><p>Bitte geben Sie Ihre Anmeldedaten erneut ein.</p>");
}
