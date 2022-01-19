#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
//#include <QtSqlDatabase>
#include <QMessageBox>
#include "loggedinscreen.h"
#include<QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void changeToMainMenu();

    void on_lineEditID_returnPressed();

    void on_lineEditID_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;


    QSqlDatabase database;
};
#endif // MAINWINDOW_H
