#include "loggedinscreen.h"
#include "ui_loggedinscreen.h"

LoggedInScreen::LoggedInScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggedInScreen)
{
    ui->setupUi(this);
}

LoggedInScreen::~LoggedInScreen()
{
    delete ui;
}
