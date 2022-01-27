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

private slots:

  void on_rabattAnRadio_toggled();

  void on_rabattAusRadio_toggled();

  void on_rabattSlider_sliderMoved(int position);

  void on_rabattAusRadio_clicked();

private:
  Ui::invoiceCreation *ui;
};

#endif // INVOICECREATION_H
