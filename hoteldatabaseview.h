#ifndef HOTELDATABASEVIEW_H
#define HOTELDATABASEVIEW_H

#include <QDialog>

namespace Ui {
class hotelDatabaseView;
}

class hotelDatabaseView : public QDialog
{
    Q_OBJECT

public:
    explicit hotelDatabaseView(QWidget *parent = nullptr);
    ~hotelDatabaseView();

private slots:

    void on_aktualisierenButton_clicked();

    void on_eintragenButton_clicked();

    void on_suchenButton_clicked();

private:
    Ui::hotelDatabaseView *ui;
};

#endif // HOTELDATABASEVIEW_H
