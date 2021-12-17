#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include <QDialog>

namespace Ui {
class detailsbookwindow;
}

class detailsbookwindow : public QDialog
{
    Q_OBJECT

public:
    explicit detailsbookwindow(QWidget *parent = nullptr);
    ~detailsbookwindow();

private:
    Ui::detailsbookwindow *ui;
};

#endif // BOOKDETAILSWINDOW_H
