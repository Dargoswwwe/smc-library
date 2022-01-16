#include "bookdetailswindow.hpp"

#include "ui_bookdetailswindow.h"

BookDetailsWindow::BookDetailsWindow(Book book, QWidget* parent)
    : QDialog(parent)
    , book(book)
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

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &BookDetailsWindow::imageDownloaded);
    manager->get(QNetworkRequest(QUrl(book.getUrl().c_str())));

    setMainWindow();
    connect(mainWindow, &MainWindow::receivedBorrowDate, this, &BookDetailsWindow::setBorrowedDate);

    mainWindow->getBorrowedDate(book.getTitle().c_str());
}

void BookDetailsWindow::setMainWindow()
{
    QObject* parent;
    for (parent = this->parent(); !dynamic_cast<MainWindow*>(parent); parent = parent->parent()) { }
    mainWindow = dynamic_cast<MainWindow*>(parent);
}

void BookDetailsWindow::imageDownloaded(QNetworkReply* resp)
{
    QByteArray downloadedImage = resp->readAll();
    resp->deleteLater();
    bookCover.loadFromData(downloadedImage);
    ui->coverBookLabel->setPixmap(bookCover);
}

void BookDetailsWindow::setBorrowedDate(std::string bookTitle, QDate borrowedDate)
{
    if (bookTitle == book.getTitle()) this->borrowedDate = borrowedDate;
    qDebug() << borrowedDate;
}

BookDetailsWindow::~BookDetailsWindow() { delete ui; }

void BookDetailsWindow::on_borrowButton_clicked() { mainWindow->borrowBook(book.getTitle()); }

void BookDetailsWindow::on_returnButton_clicked() { mainWindow->returnBook(book.getTitle()); }
