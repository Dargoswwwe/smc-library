#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>
#include <QDate>
#include<vector>
#include"book.hpp"

class DatabaseManager {
private:
    QSqlDatabase database;
    std::string databaseDir;

public:
    DatabaseManager();

    void deleteUsersTable();
    void deleteBookTable();
    void deleteUsersBooksTable();
    void deleteBook(QString title);
    void deleteUser(QString username);
    void deleteRowFromUsersBooksTable(int id_user, int id_book);

    void addValuesIntoBookTable(int id, QString title, QString authors, QString language, int original_publication_year,
                                float avarage_rating, int ratings_count, QString isbn, QString image_url,int available_books);
    void addValuesIntoUsersTable(QString username, QString password);
    void addValuesIntoUsersBooksTable(int user_id, int book_id);

    void changeUserPassword(QString username, QString password);
    void changeUsername(QString username, QString password);

    void insertBooksIntoDataBase();
    void updateAvailableBook(QString title, int available_books);
    void decreaseAvailableBook(QString title);
    void increaseAvailableBook(QString title);

    void displayUsersForBook(int book_id);
    void displayBorrowedBooksForUser(int user_id);
    void displayAllBooks();

    int countBooks();
    bool validUsername(QString username);
    bool validPassword(QString username, QString password);

    std::vector<Book> createBooksArray();
};

#endif // DATABASE_MANAGER_H
