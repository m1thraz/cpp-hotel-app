#ifndef INVOICECREATION_H
#define INVOICECREATION_H

#include <QDialog>
#include "displayinvoice.h"

namespace Ui {
  class invoiceCreation;
}

class invoiceCreation : public QDialog
{
  Q_OBJECT

public:
  explicit invoiceCreation(QWidget *parent = nullptr);
  ~invoiceCreation();

  bool lineEditVerification(const int buttontyp);
  void setBuchungsID(int buchungsID);
  int getBuchungsID();
  void setAnmerkungen(std::string anmerkungen);
  std::string getAnmerkungen();
  void setInvoivedata(std::vector<std::string> &);
  std::vector<std::string> getInvoicedata();

private slots:

  void on_rabattAnRadio_toggled();

  void on_rabattAusRadio_toggled();

  void on_rabattSlider_sliderMoved(int position);

  void on_pushButtonModRechnungUpdaten_clicked();

  void on_pushButtonRechnungErstellen_clicked();

  void on_pushButtonRechnungAnzeigen_clicked();

private:
  Ui::invoiceCreation *ui;
  int buchungsID;
  std::string anmerkungen;

  std::vector<std::string> invoicedata;

//  displayinvoice *invoicedisplay;

};

#endif // INVOICECREATION_H
