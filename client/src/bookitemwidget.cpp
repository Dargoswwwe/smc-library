#include "bookitemwidget.hpp"

#include "bookdetailswindow.h"
#include "ui_bookitemwidget.h"

BookItemWidget::BookItemWidget(Book& book, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::BookItemWidget)
{
    ui->setupUi(this);
    ui->titleLabel->setText(book.getTitle().c_str());
    QString authors;
    for (unsigned long long index = 0; index < book.getAuthors().size(); index++) {
        authors.push_back(book.getAuthors()[index].c_str());
        if (index != book.getAuthors().size() - 1) authors.push_back(", ");
    }
    ui->authorLabel->setText(authors);

    QString rating = QString::number(book.getAverageRating());
    ui->ratingLabel->setText(rating);
}

BookItemWidget::~BookItemWidget() { delete ui; }

void BookItemWidget::on_detailsButton_clicked()
{
    detailsbookwindow detailsBookWindow;
    detailsBookWindow.setModal(true);
    detailsBookWindow.exec();
}
