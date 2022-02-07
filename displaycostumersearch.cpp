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

void displaycostumersearch::changeTextCostumerInfo()
{
    costumerDatabaseView *costumerObject = new costumerDatabaseView();

    std::string checkcostumerInput;
    for (auto& s:costumerObject->costumerdata) {
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
