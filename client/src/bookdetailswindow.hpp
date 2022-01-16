#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include "book.hpp"
#include "mainwindow.hpp"

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class BookDetailsWindow;
}

class BookDetailsWindow : public QDialog {
    Q_OBJECT

private:
    Book book;
    QPixmap bookCover;
    MainWindow* mainWindow;
    std::optional<QDate> borrowedDate;

    void setMainWindow();

public slots:
    void imageDownloaded(QNetworkReply* response);
    void on_borrowButton_clicked();
    void setBorrowedDate(std::string bookTitle, QDate borrowedDate);

public:
    explicit BookDetailsWindow(Book book, QWidget* parent = nullptr);
    ~BookDetailsWindow();

private slots:
    void on_returnButton_clicked();

private:
    Ui::BookDetailsWindow* ui;
};

#endif // BOOKDETAILSWINDOW_H
