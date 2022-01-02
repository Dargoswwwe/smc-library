#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "book.hpp"

#include <QDate>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtNetwork>
#include <vector>

class DatabaseManager
{
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
    void deleteRowFromUsersBooksTable(int userId, int bookId);

    bool addValuesIntoBookTable(int id, QString title, QString authors, QString language, int originalPublicationYear,
        float averageRating, int ratingsCount, QString isbn, QString imageUrl, int availableBooks);
    bool addValuesIntoUsersTable(QString username, QString password);
    bool addValuesIntoUsersBooksTable(int userId, int bookId);

    void changeUserPassword(QString username, QString password);
    void changeUsername(QString username, QString password);

    void insertBooksIntoDataBase();
    void updateAvailableBook(QString title, int availableBooks);
    void decreaseAvailableBook(QString title);
    void increaseAvailableBook(QString title);

    void getUsersForBook(int bookId);
    // std::vector<std::pair<Book, std::string>> getBorrowedBooksForUser(int userId);
    std::vector<Book> getBorrowedBooksForUser(int userId);
    // void getAllBooks();
    std::vector<Book> getAllBooks();
    int getUserId(QString username);

    int countBooks();
    bool validUsername(QString username);
    bool validPassword(QString username, QString password);
    bool uniquePassword(QString password);

    std::vector<Book> createBooksArray();
};

#endif // DATABASE_MANAGER_H
