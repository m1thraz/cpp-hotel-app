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
    std::vector<std::string> getHoteldata();

private:
    Ui::displayhotelsearch *ui;

    std::vector<std::string> hoteldata;

};

#endif // DISPLAYHOTELSEARCH_H
