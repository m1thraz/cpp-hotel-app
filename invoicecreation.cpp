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





void invoiceCreation::on_rabattSlider_sliderMoved(int position)
{
ui->rabattBar->setValue(position);
}

void invoiceCreation::on_rabattAnRadio_toggled()
{
  ui->rabattSlider->setEnabled(true);
  ui->rabattSlider->show();

}

void invoiceCreation::on_rabattAusRadio_toggled()
{

    ui->rabattSlider->setDisabled(true);
    ui->rabattBar->setValue(0);
    ui->rabattSlider->setValue(0);

}


