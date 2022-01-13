#include "checkincheckoutscreen.h"
#include "ui_checkincheckoutscreen.h"

checkInCheckOutScreen::checkInCheckOutScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkInCheckOutScreen)
{
    ui->setupUi(this);
}

checkInCheckOutScreen::~checkInCheckOutScreen()
{
    delete ui;
}
