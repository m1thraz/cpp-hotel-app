#ifndef COSTUMERDATABASEVIEW_H
#define COSTUMERDATABASEVIEW_H

#include <QDialog>

namespace Ui {
class costumerDatabaseView;
}

class costumerDatabaseView : public QDialog
{
    Q_OBJECT

public:
    explicit costumerDatabaseView(QWidget *parent = nullptr);
    ~costumerDatabaseView();

private:
    Ui::costumerDatabaseView *ui;
};

#endif // COSTUMERDATABASEVIEW_H
