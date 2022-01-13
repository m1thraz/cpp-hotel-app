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

private:
    Ui::hotelDatabaseView *ui;
};

#endif // HOTELDATABASEVIEW_H
