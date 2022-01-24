#include "infomessage.h"
#include "ui_infomessage.h"

infomessage::infomessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infomessage)
{
    ui->setupUi(this);
}

infomessage::~infomessage()
{
    delete ui;
}

void infomessage::changeTextModifiziert() {
    ui->labelInfoMessage->setText("<p>Datensatz wurde erfolgreich modifiziert.</p>");
}

void infomessage::changeTextNeu() {
    ui->labelInfoMessage->setText("<p>Neuer Eintrag wurde erstellt.</p>");
}

void infomessage::on_closeWindowButton_clicked() {
    this->hide();
}
