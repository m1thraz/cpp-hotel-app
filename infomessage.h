#ifndef INFOMESSAGE_H
#define INFOMESSAGE_H

#include <QDialog>

namespace Ui {
  class infomessage;
}

class infomessage : public QDialog
{
  Q_OBJECT

public:
  explicit infomessage(QWidget *parent = nullptr);
  ~infomessage();
  void changeTextModifiziert();
  void changeTextNeu();
  void changeTextDelete();

private slots:
  void on_closeWindowButton_clicked();

private:
  Ui::infomessage *ui;
};

#endif // INFOMESSAGE_H
