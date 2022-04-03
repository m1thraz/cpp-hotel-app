#ifndef DISPLAYCOSTUMERSEARCH_H
#define DISPLAYCOSTUMERSEARCH_H

#include <QDialog>
#include "costumerdatabaseview.h"

namespace Ui {
  class displaycostumersearch;
}

class displaycostumersearch : public QDialog
{
  Q_OBJECT

public:
  explicit displaycostumersearch(QWidget *parent = nullptr);
  ~displaycostumersearch();

  void changeTextCostumerInfo();

  displaycostumersearch(std::vector<std::string>& data);
  std::vector<std::string> costumerdata;

private slots:
  void on_closeWindowBtn_clicked();

private:
  Ui::displaycostumersearch *ui;

  costumerDatabaseView *costumerObject;
};

#endif // DISPLAYCOSTUMERSEARCH_H
