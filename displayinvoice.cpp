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

displayinvoice::displayinvoice(std::vector<std::string> &data) {
    this->invoicedata=data;
}

void displayinvoice::changeTextInvoiceInfo()
{

    std::string checkinvoiceInput;
    for (auto& s:invoicedata) {
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
