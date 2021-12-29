#include "bookdetailswindow.hpp"

#include "ui_bookdetailswindow.h"

BookDetailsWindow::BookDetailsWindow(Book book, QWidget* parent)
    : book(book)
    , QDialog(parent)
    , ui(new Ui::BookDetailsWindow)
{
    ui->setupUi(this);

    ui->titleLabel->setText(book.getTitle().c_str());
    ui->authorsLabel->setText(ui->authorsLabel->text() + book.getAuthorsString().c_str());
    ui->languageLabel->setText(ui->languageLabel->text() + book.getLanguage().c_str());
    ui->publYearLabel->setText(ui->publYearLabel->text() + QString::number(book.getOriginalPublication()));
    ui->ratingLabel->setText(ui->ratingLabel->text() + QString::number(book.getAverageRating()));
    ui->ratingsCountLabel->setText(ui->ratingsCountLabel->text() + QString::number(book.getRatingsCount()));
    ui->contentLabel->setText(book.getContent().c_str());
}

BookDetailsWindow::~BookDetailsWindow() { delete ui; }
