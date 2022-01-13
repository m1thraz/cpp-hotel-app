#include "costumerdatabaseview.h"
#include "ui_costumerdatabaseview.h"

costumerDatabaseView::costumerDatabaseView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::costumerDatabaseView)
{
    ui->setupUi(this);
}

costumerDatabaseView::~costumerDatabaseView()
{
    delete ui;
}
