#ifndef LOGGEDINSCREEN_H
#define LOGGEDINSCREEN_H

#include <QDialog>

namespace Ui {
class LoggedInScreen;
}

class LoggedInScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoggedInScreen(QWidget *parent = nullptr);
    ~LoggedInScreen();

private:
    Ui::LoggedInScreen *ui;
};

#endif // LOGGEDINSCREEN_H
