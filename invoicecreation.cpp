#include "invoicecreation.h"
#include "ui_invoicecreation.h"

invoiceCreation::invoiceCreation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::invoiceCreation)
{
    ui->setupUi(this);
}

invoiceCreation::~invoiceCreation()
{
    delete ui;
}
