#include "displaycostumersearch.h"
#include "ui_displaycostumersearch.h"

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
