#include "bookroomview.h"
#include "ui_bookroomview.h"

bookroomview::bookroomview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookroomview)
{
    ui->setupUi(this);
}

bookroomview::~bookroomview()
{
    delete ui;
}
