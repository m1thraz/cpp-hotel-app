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
  int getMID();
  void setNachname(std::string nachname);
  std::string getNachname();
  void setVorname(std::string vorname);
  std::string getVorname();
  void setPW(std::string vorname);
  std::string getPW();
  bool lineEditVerification(const int button);



private slots:
  void on_pushButtonNeuerEintrag_clicked();




private:
    Ui::workerview *ui;

    int mID;
    std::string nachname;
    std::string vorname;
    std::string pw;


public:

    std::vector<std::string> workerdata;

};

#endif // WORKERVIEW_H
