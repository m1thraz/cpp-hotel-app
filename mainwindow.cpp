#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    //This is mysql should be checked if this is correct with our database that we need to use!

    database = QSqlDatabase::addDatabase("QMYSQL", "MyConnect");
    database.setHosteName("localhost");
    database.setUserName("root");
    database.setPasword("");
    database.setDatabaseName("qt5register");
}
