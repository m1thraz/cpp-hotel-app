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

    std::string checkcostumerInput;
    for (auto& s:costumerdata) {
        checkcostumerInput+=s;
    }
    qDebug() << "Folgende Daten wurden übergeben: " << checkcostumerInput.c_str();

    QString str = QString::fromUtf8(checkcostumerInput.c_str());

    ui->displaySearchResultsLabel->setText(str);
}

void displaycostumersearch::on_closeWindowBtn_clicked()
{
    this->hide();
}
