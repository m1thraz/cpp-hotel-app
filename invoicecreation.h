#ifndef INVOICECREATION_H
#define INVOICECREATION_H

#include <QDialog>

namespace Ui {
class invoiceCreation;
}

class invoiceCreation : public QDialog
{
    Q_OBJECT

public:
    explicit invoiceCreation(QWidget *parent = nullptr);
    ~invoiceCreation();

private:
    Ui::invoiceCreation *ui;
};

#endif // INVOICECREATION_H
