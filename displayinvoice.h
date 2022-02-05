#ifndef DISPLAYINVOICE_H
#define DISPLAYINVOICE_H

#include <QDialog>

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
};

#endif // DISPLAYINVOICE_H
