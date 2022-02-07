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

std::vector<std::string> displayinvoice::getInvoicedata() {
    return this->invoicedata;
}
