#include "displayinvoice.h"
#include "ui_displayinvoice.h"
#include <QDebug>

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

void displayinvoice::changeTextInvoiceInfo() {

    invoiceCreation *invoiceObject = new invoiceCreation();

    std::string checkinvoiceInput;
    for (auto& s:invoiceObject->invoicedata) {
        checkinvoiceInput+=s;
    }
    qDebug() << "Folgende Daten wurden gespeichert: " << checkinvoiceInput.c_str();

    QString str = QString::fromUtf8(checkinvoiceInput.c_str());

    ui->displaySearchResultsLabel->setText(str);
}

void displayinvoice::on_closeWindowBtn_clicked()
{
    this->hide();
}
