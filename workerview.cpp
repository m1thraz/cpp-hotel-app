#include "workerview.h"
#include "ui_workerview.h"
#include "database.h"
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QDebug>
#include "errormessage.h"
#include "infomessage.h"
#include <iostream>




workerview::workerview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workerview)
{
    ui->setupUi(this);
}

workerview::~workerview()
{
    delete ui;
}



void workerview::on_buttonBox_accepted()
{

}

void workerview::on_pushButtonNeuerEintrag_clicked()
{

}
