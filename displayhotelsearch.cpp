#include "displayhotelsearch.h"
#include "ui_displayhotelsearch.h"
#include <QDebug>

displayhotelsearch::displayhotelsearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displayhotelsearch)
{
    ui->setupUi(this);
}

displayhotelsearch::~displayhotelsearch()
{
    delete ui;
}

displayhotelsearch::displayhotelsearch(std::vector<std::string> &data) {
    this->hoteldata=data;
}

void displayhotelsearch::changeTextHotelInfo()
{

    std::string checkhotelInput;
    for (auto& s:hoteldata) {
        checkhotelInput+=s;
    }
    qDebug() << "Folgende Daten wurden gespeichert: " << checkhotelInput.c_str();

//    QString str = QString::fromUtf8(checkhotelInput.c_str());

    ui->labelHotelResult->setText(checkhotelInput.c_str());
}

void displayhotelsearch::on_closeWindowBtn_clicked()
{
    this->hide();
}
