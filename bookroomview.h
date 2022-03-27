#ifndef BOOKROOMVIEW_H
#define BOOKROOMVIEW_H

#include <QDialog>

namespace Ui {
class bookroomview;
}

class bookroomview : public QDialog
{
    Q_OBJECT

public:
    explicit bookroomview(QWidget *parent = nullptr);
    ~bookroomview();

    bool lineEditVerification(const int buttontyp);
    bool verifyRoomShouldNotBeBooked();
    bool verifyRoomShouldBeBooked();
    bool bookMasageSauna(int sonderleistungsID);
    void setKundenID(int kundenID);
    int getKundenID();
    void setMitarbeiterID(int mitarbeiterID);
    int getMitarbeiterID();
    void setBestandID(int bestandID);
    int getBestandID();
    void setAnreiseDatum(std::string anreisedatum);
    std::string getAnreiseDatum();
    void setAbreiseDatum(std::string abreisedatum);
    std::string getAbreiseDatum();
    void setBuchungsID(int buchungsID);
    int getBuchungsID();
    void setMID(QString MID);


private slots:
    void on_bookRoomButton_clicked();

    void on_bookExtrasButton_clicked();

    void on_cancelBookedRoomButton_clicked();


    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_comboBox_4_currentTextChanged(const QString &arg1);



private:
    Ui::bookroomview *ui;
    int kundenID;
    int mitarbeiterID;
    int bestandID;
    std::string anreisedatum;
    std::string abreisedatum;
    int buchungsID;

};

#endif // BOOKROOMVIEW_H
