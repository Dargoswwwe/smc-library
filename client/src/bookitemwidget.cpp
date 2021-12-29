#include "bookitemwidget.hpp"

#include "bookdetailswindow.hpp"
#include "ui_bookitemwidget.h"

BookItemWidget::BookItemWidget(Book book, QWidget* parent)
    :QWidget(parent)
    , book(book)
    , ui(new Ui::BookItemWidget)
{
    ui->setupUi(this);
    ui->titleLabel->setText(book.getTitle().c_str());
    ui->authorLabel->setText(book.getAuthorsString().c_str());

    QString rating = QString::number(book.getAverageRating());
    ui->ratingLabel->setText(rating);
}

BookItemWidget::~BookItemWidget() { delete ui; }

void BookItemWidget::on_detailsButton_clicked()
{
    BookDetailsWindow bookDetailsWindow(book);
    bookDetailsWindow.setModal(true);
    bookDetailsWindow.exec();
}
