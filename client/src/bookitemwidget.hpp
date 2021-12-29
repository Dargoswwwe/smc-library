#ifndef BOOKITEMWIDGET_HPP
#define BOOKITEMWIDGET_HPP

#include "book.hpp"

#include <QWidget>

namespace Ui {
class BookItemWidget;
}

class BookItemWidget : public QWidget
{
    Q_OBJECT

private:
    Book book;

public:
    explicit BookItemWidget(Book book, QWidget* parent = nullptr);
    ~BookItemWidget();

private slots:
    void on_detailsButton_clicked();

private:
    Ui::BookItemWidget* ui;
};

#endif // BOOKITEMWIDGET_HPP
