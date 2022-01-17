#ifndef LOGGEDINSCREEN_H
#define LOGGEDINSCREEN_H

#include <QDialog>
#include "hoteldatabaseview.h"
#include "costumerdatabaseview.h"
#include "loggedinscreen.h"
#include "mainwindow.h"
#include "invoicecreation.h"

namespace Ui {
class LoggedInScreen;
}

class LoggedInScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoggedInScreen(QWidget *parent = nullptr);
    ~LoggedInScreen();

private slots:
  void on_hotelButton_clicked();

  void on_dbButton_clicked();

  void on_RechnungButton_clicked();

  void on_logoutButto_clicked();

private:
    Ui::LoggedInScreen *ui;
};

#endif // LOGGEDINSCREEN_H
