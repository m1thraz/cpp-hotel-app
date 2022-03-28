#ifndef WORKERVIEW_H
#define WORKERVIEW_H

#include <QDialog>



namespace Ui {
class workerview;
}

class workerview : public QDialog
{
    Q_OBJECT

public:
  explicit workerview(QWidget *parent = nullptr);
  ~workerview();


private slots:
  void on_pushButtonNeuerEintrag_clicked();

  void on_buttonBox_accepted();

private:
    Ui::workerview *ui;

};

#endif // WORKERVIEW_H
