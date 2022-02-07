#include "displaycostumersearch.h"
#include "ui_displaycostumersearch.h"
#include "costumerdatabaseview.h"
#include <QDebug>


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
    costumerDatabaseView costumerObject = new costumerDatabaseView();

//    QString str = QString::fromUtf8(costumerdata);
           std::string checkcostumerInput;
           for (auto& s:costumerObject.costumerdata) {
                checkcostumerInput+=s;
            }
            qDebug() << "Folgende Daten wurden gespeichert: " << checkcostumerInput.c_str();

//    ui->displaySearchResultsLabel->setText(str);
}

void displaycostumersearch::on_closeWindowBtn_clicked()
{
    this->hide();
}
