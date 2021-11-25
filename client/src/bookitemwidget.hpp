#ifndef BOOKITEMWIDGET_HPP
#define BOOKITEMWIDGET_HPP

#include <QWidget>

namespace Ui {
class BookItemWidget;
}

class BookItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookItemWidget(QWidget *parent = nullptr);
    ~BookItemWidget();

private:
    Ui::BookItemWidget *ui;
};

#endif // BOOKITEMWIDGET_HPP
