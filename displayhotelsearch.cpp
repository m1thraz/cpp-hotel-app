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
