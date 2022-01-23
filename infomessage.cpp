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

void infomessage::changeTextHotelModifiziert() {
    ui->labelInfoMessage->setText("<p>Datensatz wurde erfolgreich modifiziert.</p>");
}

void infomessage::changeTextHotelNeu() {
    ui->labelInfoMessage->setText("<p>Neuer Eintrag wurde erstellt.</p>");
}

void infomessage::on_closeWindowButton_clicked() {
    this->hide();
}
