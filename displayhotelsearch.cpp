#include "displayhotelsearch.h"
#include "ui_displayhotelsearch.h"

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

void displayhotelsearch::changeTextHotelInfo() {

    hotelDatabaseView hotelObject = new hotelDatabaseView();

    std::string checkhotelInput;
    for (auto& s:hotelObject.hoteldata) {
         checkhotelInput+=s;
     }
     qDebug() << "Folgende Daten wurden gespeichert: " << checkhotelInput.c_str();
}
