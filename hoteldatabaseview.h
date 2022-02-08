#ifndef HOTELDATABASEVIEW_H
#define HOTELDATABASEVIEW_H

#include <QDialog>
class displayhotelsearch;

namespace Ui {
class hotelDatabaseView;
}

class hotelDatabaseView : public QDialog
{
    Q_OBJECT

public:
    explicit hotelDatabaseView(QWidget *parent = nullptr);
    ~hotelDatabaseView();

    bool lineEditVerification(const int buttontyp);
    void modPreis();
    void modZimmer();
    void setZimmerID(int zimmerID);
    int getZimmerID();
    void setZimmernummer(int zimmernummer);
    int getZimmernummer();
    void setPreis(int preis);
    int getPreis();
    void setAnreiseDatum(std::string anreisedatum);
    std::string getAnreiseDatum();
    void setAbreiseDatum(std::string abreisedatum);
    std::string getAbreiseDatum();
    void removeRoomEquipment();
    void setHoteldata(std::vector<std::string> &);
    std::vector<std::string> getHoteldata();

private slots:

    void on_aktualisierenButton_clicked();

    void on_eintragenButton_clicked();

    void on_suchenButton_clicked();

    void on_radioButtonAbfrVerfuegbar_toggled(bool checked);

    void on_entfernenButton_clicked();

private:
    Ui::hotelDatabaseView *ui;

    int zimmerID;
    int zimmernummer;
    int preis;
    std::string anreisedatum;
    std::string abreisedatum;

public:

    std::vector<std::string> hoteldata;

    displayhotelsearch *hotelsearch;
};

#endif // HOTELDATABASEVIEW_H
