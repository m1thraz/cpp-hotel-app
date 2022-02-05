# C++ Hotel-App
Von <br>
Kevin Thölken  &nbsp;  5052398 <br>
Tabea Hawley	&nbsp;   	5140840 <br>
Hanna Kühl  &nbsp;   &nbsp; &nbsp;      5136670 <br>


<h2> Datenbank Aufbau </h2>

![DatenbankAufbau](https://user-images.githubusercontent.com/49205127/152532371-b63efbef-acd4-4d5a-b590-a5a569099330.png)

<h2> Anleitung </h2>

<h3> Datenbank </h3>
Die Datenbank wird während der Laufzeit erstellt und ist zu finden in dem Build-Ordner und hat den Namen "hotel-app.sqlite". Die Datei kann mit dem <a href="https://sqlitebrowser.org/">DB Browser</a> eingesehen werden. Sollte es Probleme mit den Funktionen geben, die sql Datei löschen und neu generieren lassen.

<h3> Login </h3>

 Daten Zum einloggen: <br>
 Anmeldename: 12345 <br>
 Passwort: myPassword
 
<h3> Anleitung zum Benutzen der Fenster </h3>
 
<h4> Hauptfenster </h4>



![pen-testin1g](https://user-images.githubusercontent.com/49205127/152649480-6aaadd62-169f-4d04-927f-011df58149a9.png)

Nachdem Login öffnet sich das Hauptfenster. Diese Fenster dient der weiteren Navigation in der Applikation. Wird ein Button geklickt und somit ein neues Fenster geöffnet, bleibt dieses Fenster dennoch bestehen, sodass auch mehrer Fenster gleichzeitig geöffnet werden können
  
<h4> Hoteldatenbank </h4>


![Screenshot 2022-02-05 165922](https://user-images.githubusercontent.com/49205127/152649326-198d149c-7e23-4320-a01e-9d4cea215c67.png)

Dieses Fenster dient zu bearbeitung der Räume und Raumbuchungen. Im ersten Reiter ( Datenbank abfrage) können Zimmer mit verschiedenen Kriterien gesucht werden. Nachdem diese eingetragen sind kann mit dem Suchen button eine Datenbank abfrage gestartet werden (work in progress)

Der zweite Reiter(Datenbank modifizieren) dient zu Modifikation der Räume. Dieser Reiter besteht aus zwei Teilen. Der Obere dient dazu alle Räume einer Zimmer ID( Doppel - oder Einzelbett) den Preis verändern zu können. Der zweite Teil bietet Möglicvhkeiten einzelne zimmer in ihrer Austattung anzupassen.

In dem dritten Reiter( Neuer Datenbankeintrag) können neue zimmer erstellt werden. 


<h4> Kundendatenbank </h4>


![Screenshot 2022-02-05 165947](https://user-images.githubusercontent.com/49205127/152649329-7f7d4e1a-e121-475b-ae54-55f469fac645.png)

In diesem Fenster können Kunden angelegt, gesucht und modifiziert werden.
Der erste Reiter(Datenbank abfrage) dient der Suche einzelner Kunden. Der zweite Reiter (Datenbank modifizieren) wird genutzt um Kunden in der datenbank bearbeiten zu können und der Dritte(Neuer Datenbankeintrag) wird genutzt um neue Kunden zu erstellen.
 
<h4> Rechnungerstellen </h4>



![rechnung](https://user-images.githubusercontent.com/49205127/152649317-c34353ac-aa46-4dbd-901b-eb30198f2013.png)

Dieses Fenster dient zur Erstellung der Rechnungen. Eine Rechnung kann erst dann erstellt werden wenn der Kunde vorher augechekct wurde (siehe "Ein- und Ausscheken").
In dem ersten Te3il (Rechnung erstellen) kann eine BuchungsId geladen werden und mit prozentualen Vergünstigungen versehen werden, um daraus die rechnung erstellen und speicher zu können. In der Rechnung werden die Zimmerbuchungen einer Person, mit den von ihr bekommenen Sonderleistungen zusammengerechnet und abschließent wird die eventuelle Vergünstigung abgezogen. 

In dem zweiten Teil(Rechnungsvermerk ändern) können nächtragliche Anmerkungern einer rechnung hinzugefügt werden. Dies ist z.B. sinnvoll wenn im nachinein Beschädigungen am Zimmer vorgefunden werden.

Der dritte Teil dient dazu eine Rechnung zu laden und anzuzeigen (work in progress)




<h4> Zimmer buchen </h4>

![hotelManagement](https://user-images.githubusercontent.com/49205127/152649321-03ab958b-4c5c-4569-95c2-39eb1b9122c4.png)

Dies ist das Fenster in dem die Buchungen stattfinden. 
Im ersten Tel (Zimmerbuchung) können für Kunden Zimmer gebucht werden. Hierzu werden die KundenID, die MitarbeiterID und die BestandID(Zimmer) eingegeben werden  und eine Anreise - und Abreisedatum eingetragen werden. Ist das Zimmer bereits ausgebucht ist die Buchung nicht möglich. Diese Teil bietet auch die Möglichkeit eine Buchung zu löschen, die benötigten Informationen sind die selben wie beim Buchungsvorgang.

Der zweite Teil dient der Buchung von Sonderleistungen wie die Sauna und einer Massage. Die benötigten Informationen sind die selben wie bei der Zimmerbuchung. Es können beliebig viele Sonderleistungen gebucht pro Kunde gebucht werden.

<h4> Ein- und Ausschecken </h4>

![checkin](https://user-images.githubusercontent.com/49205127/152649284-2848254e-aaea-4964-abe4-4bac64171663.png)

Fenster zum Ein- und Auschecken der Kunden. Im ersten Teil findet der Check In statt. Hierzu werden die Kunden ID und die BuchungsID benötigt, der Kunde kaann somit nur eingeschcekct werden, wenn er vorher reserveiert hat und eine BuchungsId  besitzt.

Der zweite Teil dient dem Check Out und die Anwendung ist ähnlich wie beim Check In. Der Hauptunterschied besteht darin, dass beim ausschecken, dass hier beschädigte Gegestände und eine extra Reinigung vermerkt werden können. Nur Kunden die vorher eingechekct wurden können ausgecheckt werden.
 
<h4> Logout </h4>
Springe zurück zum Login Fenster
 
 
 
 
