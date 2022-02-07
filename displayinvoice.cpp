#include "displayinvoice.h"
#include "ui_displayinvoice.h"

displayinvoice::displayinvoice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displayinvoice)
{
    ui->setupUi(this);
}

displayinvoice::~displayinvoice()
{
    delete ui;
}
