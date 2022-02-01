#ifndef ERRORMESSAGE_H
#define ERRORESSAGE_H

#include <QDialog>

namespace Ui {
class errormessage;
}

class errormessage : public QDialog
{
    Q_OBJECT

public:
    explicit errormessage(QWidget *parent = nullptr);
    ~errormessage();
    void changeTextIDWrong();
    void changeTextLoginError();
    void changeTextZimmernummerWrong();
    void changeTextZimmernummerExists();
    void changeTextZimmerIDWrong();
    void changeTextPreisWrong();
    void changeTextMissingModifications();
    void changeTextUpdateError();
    void changeTextZimmernummerError();
    void changeTextZimmertypError();
    void changeTextZimmertypNotSelected();
    void changeTextPreisError();
    void changeTextZimmerIDError();
    void changeTextMissingInputText();
    void changeTextDataCreationError();
    void changeTextDBRequestError();
    void changeTextKundenIDWrong();
    void changeTextNachnameWrong();
    void changeTextVornameWrong();
    void changeTextStrasseWrong();
    void changeTextHausnummerWrong();
    void changeTextWohnortWrong();
    void changeTextPlzWrong();
    void changeTextTelefonnummerWrong();
    void changeTextKundenIDError();
    void changeTextMissingCustomerArguments();
    void changeTextKundenIDDoesntExist();
    void changeTextMitarbeiterIDWrong();
    void changeTextBestandIDWrong();
    void changeTextMitarbeiterIDDoesntExist();
    void changeTextBestandIDDoesntExist();
    void changeTextRoomIsBooked();
    void changeTextMissingSpecialService();
    void changeTextDeleteDataError();
    void changeTextCancelBookingError();
    void changeTextBuchungsIDError();
    void changeTextBuchungDoesntExist();

private slots:
    void on_closeWindowButton_clicked();

private:
    Ui::errormessage *ui;
};

#endif // ERRORMESSAGE_H
