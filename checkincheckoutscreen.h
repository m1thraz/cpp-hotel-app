#ifndef CHECKINCHECKOUTSCREEN_H
#define CHECKINCHECKOUTSCREEN_H

#include <QDialog>

namespace Ui {
class checkInCheckOutScreen;
}

class checkInCheckOutScreen : public QDialog
{
    Q_OBJECT

public:
    explicit checkInCheckOutScreen(QWidget *parent = nullptr);
    ~checkInCheckOutScreen();

    bool lineEditVerification(const int buttontyp);
    void setKundenID(int kundenID);
    int getKundenID();
    void setBuchungsID(int buchungsID);
    int getBuchungsID();

private slots:


    void on_checkInButton_clicked();

    void on_checkOutBtn_clicked();

private:
    Ui::checkInCheckOutScreen *ui;
    int kundenID;
    int buchungsID;
};

#endif // CHECKINCHECKOUTSCREEN_H
