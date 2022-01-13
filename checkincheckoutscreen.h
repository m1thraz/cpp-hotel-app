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

private:
    Ui::checkInCheckOutScreen *ui;
};

#endif // CHECKINCHECKOUTSCREEN_H
