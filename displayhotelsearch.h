#ifndef DISPLAYHOTELSEARCH_H
#define DISPLAYHOTELSEARCH_H

#include <QDialog>
#include "hoteldatabaseview.h"

namespace Ui {
  class displayhotelsearch;
}

class displayhotelsearch : public QDialog
{
  Q_OBJECT

public:
  explicit displayhotelsearch(QWidget *parent = nullptr);
  ~displayhotelsearch();

  void changeTextHotelInfo();

  displayhotelsearch(std::vector<std::string>& data);
  std::vector<std::string> hoteldata;

private slots:
  void on_closeWindowBtn_clicked();

private:
  Ui::displayhotelsearch *ui;

  hotelDatabaseView * hotelObject;

};

#endif // DISPLAYHOTELSEARCH_H
