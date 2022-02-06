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

private:
    int *idInt;
    std::string *vornameString;
    std::string *nachnameString;
    std::string *strasseString;
    int *hausnummerInt;
    std::string *wohnortString;
    int *plzInt;
    int *telefonnummerInt;
    std::string *emailString;
};

#endif // DISPLAYCOSTUMERSEARCH_H
