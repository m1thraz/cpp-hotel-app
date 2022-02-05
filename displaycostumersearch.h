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

private:
    Ui::displaycostumersearch *ui;
};

#endif // DISPLAYCOSTUMERSEARCH_H
