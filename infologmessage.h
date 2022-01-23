#ifndef INFOLOGMESSAGE_H
#define INFOLOGMESSAGE_H

#include <QDialog>

namespace Ui {
class infologmessage;
}

class infologmessage : public QDialog
{
    Q_OBJECT

public:
    explicit infologmessage(QWidget *parent = nullptr);
    ~infologmessage();
    void changeTextIDWrong();

private slots:
    void on_closeWindowButton_clicked();

private:
    Ui::infologmessage *ui;
};

#endif // INFOLOGMESSAGE_H
