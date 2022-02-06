#ifndef COSTUMERDATABASEVIEW_H
#define COSTUMERDATABASEVIEW_H

#include <QDialog>
#include "displaycostumersearch.h"

namespace Ui {
class costumerDatabaseView;
}

class costumerDatabaseView : public QDialog
{
    Q_OBJECT

public:
    explicit costumerDatabaseView(QWidget *parent = nullptr);
    ~costumerDatabaseView();

    bool lineEditVerification(const int buttontyp);
    void setKundenID(int kundenID);
    int getKundenID();
    void setNachname(std::string nachname);
    std::string getNachname();
    void setVorname(std::string vorname);
    std::string getVorname();
    void setStrasse(std::string strasse);
    std::string getStrasse();
    void setHausnummer(int hausnummer);
    int getHausnummer();
    void setWohnort(std::string wohnort);
    std::string getWohnort();
    void setPlz(int plz);
    int getPlz();
    void setTelefonnummer(int telefonnummer);
    int getTelefonnummer();
    void setEmail(std::string email);
    std::string getEmail();

private slots:
    void on_pushButtonSuchen_clicked();

    void on_pushButtonAktualisieren_clicked();

    void on_pushButtonNeuerEintrag_clicked();


private:
    Ui::costumerDatabaseView *ui;

    displaycostumersearch *costumersearch;

    int kundenID;
    std::string nachname;
    std::string vorname;
    std::string strasse;
    int hausnummer;
    std::string wohnort;
    int plz;
    int telefonnummer;
    std::string email;

public:
//    int *idInt;
//    std::string vornameString;
//    std::string nachnameString;
//    std::string strasseString;
//    int hausnummerInt;
//    std::string wohnortString;
//    int plzInt;
//    int telefonnummerInt;
//    std::string emailString;

    std::vector<std::string> costumerdata;
};

#endif // COSTUMERDATABASEVIEW_H
