#ifndef BOOKROOMVIEW_H
#define BOOKROOMVIEW_H

#include <QDialog>

namespace Ui {
class bookroomview;
}

class bookroomview : public QDialog
{
    Q_OBJECT

public:
    explicit bookroomview(QWidget *parent = nullptr);
    ~bookroomview();

private:
    Ui::bookroomview *ui;
};

#endif // BOOKROOMVIEW_H
