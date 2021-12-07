#ifndef BOOKITEMWIDGET_HPP
#define BOOKITEMWIDGET_HPP

#include <QWidget>
#include "book.hpp"
namespace Ui {
class BookItemWidget;
}

class BookItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookItemWidget(Book &book,QWidget* parent = nullptr);
    ~BookItemWidget();

private:
    Ui::BookItemWidget* ui;
};

#endif // BOOKITEMWIDGET_HPP
