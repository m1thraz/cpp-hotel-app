#include "displaycostumersearch.h"
#include "ui_displaycostumersearch.h"
#include <QDebug>

displaycostumersearch::displaycostumersearch(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::displaycostumersearch)
{
  ui->setupUi(this);
}

displaycostumersearch::~displaycostumersearch()
{
  delete ui;
}

displaycostumersearch::displaycostumersearch(std::vector<std::string> &data) {
  this->costumerdata=data;
}

void displaycostumersearch::changeTextCostumerInfo()
{

  std::string checkcostumerDelivery;
  for (auto& s:costumerdata) {
      checkcostumerDelivery+=s;
    }
  qDebug() << "Folgende Daten wurden übergeben: " << checkcostumerDelivery.c_str();

  //    QString str = QString('Ergebnis: %1').arg(checkcostumerInput.c_str());

  ui->labelCostumerResult->setText(checkcostumerDelivery.c_str());
}

void displaycostumersearch::on_closeWindowBtn_clicked()
{
  this->hide();
}
