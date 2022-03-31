#include "errormessage.h"
#include "ui_errormessage.h"

errormessage::errormessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errormessage)
{
    ui->setupUi(this);
}

errormessage::~errormessage()
{
    delete ui;
}

void errormessage::on_closeWindowButton_clicked()
{
    this->hide();
}

void errormessage::changeTextIDWrong() {
    ui->labelErrorMessage->setText("<p>Ihre Login-ID ist fehlerhaft.</p><p>Bitte geben Sie Ihre Anmeldedaten erneut ein.</p>");
}

void errormessage::changeTextLoginError() {
    ui->labelErrorMessage->setText("<p>Ihre Login-ID oder Ihr eingegebenes Passwort sind<br>falsch.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextZimmernummerWrong() {
     ui->labelErrorMessage->setText("<p>Ihre eingegebene Zimmernummer ist fehlerhaft.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextZimmerIDWrong() {
    ui->labelErrorMessage->setText("<p>Ihre eingegebene ZimmerID ist fehlerhaft.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextPreisWrong() {
    ui->labelErrorMessage->setText("<p>Der angegebene Preis muss eine Ganzzahl sein.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextMissingModifications() {
    ui->labelErrorMessage->setText("<p>Bitte geben Sie neben der Zimmernummer die<br>Ausstattungswünsche an.</p>");
}

void errormessage::changeTextUpdateError() {
    ui->labelErrorMessage->setText("<p>Beim aktualisieren der Datenbank ist ein Fehler<br>aufgetreten.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextZimmernummerError() {
    ui->labelErrorMessage->setText("<p>Das Zimmer mit der angegebenen Zimmernummer<br>existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextZimmernummerExists() {
    ui->labelErrorMessage->setText("<p>Das Zimmer mit der angegebenen Zimmernummer<br>existiert bereits.</p>");
}

void errormessage::changeTextZimmertypError() {
     ui->labelErrorMessage->setText("<p>Das gleichzeitige Auswählen von <em>Einzelbett</em> und<br><em>Doppelbett</em> ist nicht zulässig.</p>"
                                    "<p>Bitte wählen Sie nur einen Zimmertyp aus.</p>");
}

void errormessage::changeTextZimmertypNotSelected() {
    ui->labelErrorMessage->setText("<p>Bitte wählen Sie mindestens <em>Einzelbett</em> oder<br><em>Doppelbett</em> für einen neuen Eintrag aus.</p>");
}

void errormessage::changeTextPreisError() {
    ui->labelErrorMessage->setText("<p>Bitte geben Sie für eine Preisanpassung sowohl den<br>Preis, als auch eine ZimmerID ein.</p>");
}

void errormessage::changeTextZimmerIDError() {
    ui->labelErrorMessage->setText("<p>Die angegebene ZimmerID existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p<>");
}

void errormessage::changeTextMissingInputText() {
     ui->labelErrorMessage->setText("<p>Bitte überprüfen Sie Ihre Eingaben auf Vollständigkeit.</p>");
}

void errormessage::changeTextDataCreationError() {
    ui->labelErrorMessage->setText("<p>Beim Erstellen des neuen Datensatzes ist ein Fehler<br>aufgetreten.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextDBRequestError() {
    ui->labelErrorMessage->setText("<p>Beim Abrufen der Datenbank ist ein Fehler<br>aufgetreten.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextKundenIDWrong() {
    ui->labelErrorMessage->setText("<p>Die eingegebene Kunden-ID ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextNachnameWrong() {
    ui->labelErrorMessage->setText("<p>Der eingegebene Nachname ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextVornameWrong() {
    ui->labelErrorMessage->setText("<p>Der eingegebene Vorname ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextPWWrong() {
    ui->labelErrorMessage->setText("<p>Der eingegebene Passwort ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}


void errormessage::changeTextStrasseWrong() {
    ui->labelErrorMessage->setText("<p>Der eingegebene Straßenname ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextHausnummerWrong() {
    ui->labelErrorMessage->setText("<p>Die eingegebene Hausnummer ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextWohnortWrong() {
    ui->labelErrorMessage->setText("<p>Der eingegebene Wohnort ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextPlzWrong() {
    ui->labelErrorMessage->setText("<p>Die eingegebene Postleitzahl ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextTelefonnummerWrong() {
    ui->labelErrorMessage->setText("<p>Die eingegebene Telefonnummer ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextKundenIDError() {
    ui->labelErrorMessage->setText("<p>Bitte geben Sie für die Modifizierung eine Kunden-ID<br>ein.</p>");
}

void errormessage::changeTextMissingCustomerArguments() {
     ui->labelErrorMessage->setText("<p>Bitte geben Sie neben der KundenID<br>mindestens einen Veränderungswunsch an.</p>");
}

void errormessage::changeTextKundenIDDoesntExist() {
     ui->labelErrorMessage->setText("<p>Die angegebene KundenID existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p<>");
}

void errormessage::changeTextMitarbeiterIDWrong() {
     ui->labelErrorMessage->setText("<p>Die eingegebene Mitarbeiter-ID ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextBestandIDWrong() {
    ui->labelErrorMessage->setText("<p>Die eingegebene Bestand-ID ist fehlerhaft.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextMitarbeiterIDDoesntExist() {
    ui->labelErrorMessage->setText("<p>Die angegebene MitarbeiterID existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p<>");
}

void errormessage::changeTextBestandIDDoesntExist() {
    ui->labelErrorMessage->setText("<p>Die angegebene BestandID existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextRoomIsBooked() {
    ui->labelErrorMessage->setText("<p>Das ausgewählte Zimmer ist in diesem Zeitraum<br>bereits belegt.</p><p>Bitte wählen Sie ein anderes Zimmer.</p>");
}

void errormessage::changeTextMissingSpecialService() {
    ui->labelErrorMessage->setText("<p>Bitte wählen Sie mindestens eine Sonderleistung aus.</p>");
}

void errormessage::changeTextDeleteDataError() {
    ui->labelErrorMessage->setText("<p>Beim Löschen eines Datensatzes ist ein Fehler<br>aufgetreten.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextCancelBookingError() {
    ui->labelErrorMessage->setText("<p>Die zu stornierende Buchung existiert nicht.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextBuchungsIDError() {
     ui->labelErrorMessage->setText("<p>Ihre eingegebene BuchungsID ist fehlerhaft.</p><p>Bitte versuchen Sie es erneut.</p>");
}

void errormessage::changeTextBuchungDoesntExist() {
      ui->labelErrorMessage->setText("<p>Die angegebene Buchung existiert nicht.</p><p>Bitte überprüfen Sie Ihre Eingabe.</p>");
}

void errormessage::changeTextRepairNeeded() {
    ui->labelErrorMessage->setText("<p>Das gleichzeitige Reparieren und Reinigen ist nicht <br>möglich.</p>"
                                   "<p>Bitte wählen Sie entweder <em>beschädigte Gegenstände</em><br>oder <em>Reinigung</em> aus.</p>");
}

void errormessage::changeTextCheckInStatus() {
    ui->labelErrorMessage->setText("<p>Der Kunde hat bereits eingecheckt.</p>");
}

void errormessage::changeTextCheckOutStatus() {
    ui->labelErrorMessage->setText("<p>Der Kunde hat bereits ausgecheckt.</p>");
}

void errormessage::changeTextNotCheckedIn() {
    ui->labelErrorMessage->setText("<p>Der Kunde hat noch nicht eingecheckt.</p>");
}

void errormessage::changeTextNotCheckedOut() {
    ui->labelErrorMessage->setText("<p>Der Kunde hat noch nicht ausgecheckt.</p>");
}

void errormessage::changeTextRechnungExists() {
    ui->labelErrorMessage->setText("<p>Die Rechnung existiert bereits.</p>");
}

void errormessage::changeTextRechnungDoesntExist() {
    ui->labelErrorMessage->setText("<p>Zu dieser Buchung existiert noch keine Rechnung.</p>");
}
