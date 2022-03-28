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
  void setMID(int mID);


private slots:
  void on_pushButtonNeuerEintrag_clicked();
  bool lineEditVerification(const int button);

  void on_buttonBox_accepted();

private:
    Ui::workerview *ui;

    int mID;

};

#endif // WORKERVIEW_H
