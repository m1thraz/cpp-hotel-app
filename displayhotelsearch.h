#ifndef DISPLAYHOTELSEARCH_H
#define DISPLAYHOTELSEARCH_H

#include <QDialog>

namespace Ui {
class displayhotelsearch;
}

class displayhotelsearch : public QDialog
{
    Q_OBJECT

public:
    explicit displayhotelsearch(QWidget *parent = nullptr);
    ~displayhotelsearch();

private:
    Ui::displayhotelsearch *ui;

};

#endif // DISPLAYHOTELSEARCH_H
