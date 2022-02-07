#ifndef DISPLAYINVOICE_H
#define DISPLAYINVOICE_H

#include <QDialog>
#include "invoicecreation.h"

namespace Ui {
class displayinvoice;
}

class displayinvoice : public QDialog
{
    Q_OBJECT

public:
    explicit displayinvoice(QWidget *parent = nullptr);
    ~displayinvoice();

private:
    Ui::displayinvoice *ui;

    std::vector<std::string> invoicedata;
};

#endif // DISPLAYINVOICE_H
