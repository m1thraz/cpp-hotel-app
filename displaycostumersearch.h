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

    std::vector<std::string> getCostumerdata(std::vector<std::string> &);
    void changeTextCostumerInfo();

private slots:
    void on_closeWindowBtn_clicked();


private:
    Ui::displaycostumersearch *ui;

    std::vector<std::string> costumerdata;
};

#endif // DISPLAYCOSTUMERSEARCH_H
