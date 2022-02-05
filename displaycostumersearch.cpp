#include "displaycostumersearch.h"
#include "ui_displaycostumersearch.h"

displaycostumersearch::displaycostumersearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displaycostumersearch)
{
    ui->setupUi(this);

//    public slots:
//    void

}

displaycostumersearch::~displaycostumersearch()
{
    delete ui;
}

//void displaycostumersearch::changeTextCostumerInfo()
//{
//    ui->displaySearchResultsLabel->setText("<p>Bei der Suche wurden folgende Ergebnisse erzielt:</p>"
//                                           "</br>"
//                                           "<p>KundenID: " << idInt <<
//                                           );
//}
