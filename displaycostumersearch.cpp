#include "displaycostumersearch.h"
#include "ui_displaycostumersearch.h"
#include "costumerdatabaseview.h"


displaycostumersearch::displaycostumersearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displaycostumersearch)
{
    ui->setupUi(this);

//    public slots:
//    void

}

displaycostumersearch::~displaycostumersearch()
{
    delete ui;
}

void displaycostumersearch::changeTextCostumerInfo()
{

//    QString str = QString::fromUtf8(costumerdata);
//           std::string checkcostumerInput;
//           for (auto& s:costumerdata) {
//                checkcostumerInput+=s;
//            }
//            qDebug() << "Folgende Daten wurden gespeichert: " << checkcostumerInput.c_str();

//    ui->displaySearchResultsLabel->setText(str);
}

void displaycostumersearch::on_closeWindowBtn_clicked()
{
    this->hide();
}

std::vector<std::string> displaycostumersearch::getCostumerdata(std::vector<std::string> &) {
    return this->costumerdata;
}
