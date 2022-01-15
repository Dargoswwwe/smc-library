#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "book.hpp"
#include "user.hpp"

#include <QDate>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtNetwork>
#include <optional>
#include <vector>

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
    void deleteRowFromUsersBooksTable(int userId, int bookId);

    bool addValuesIntoBookTable(int id, QString title, QString authors, QString language, int originalPublicationYear,
        float averageRating, int ratingsCount, QString isbn, QString imageUrl, int availableBooks);
    bool addValuesIntoUsersTable(User const& user);
    bool addValuesIntoUsersBooksTable(int userId, int bookId);

    void changeUserPassword(QString username, QString password);
    void changeUsername(QString username, QString password);

    void insertBooksIntoDataBase();
    bool isBookAvailable(int book_id);
    void decreaseAvailableBook(QString title);
    void increaseAvailableBook(QString title);
    std::vector<Book> createBooksArray();

    std::optional<User> getUser(QString username);
    void getUsersForBook(int bookId);
    // std::vector<std::pair<Book, std::string>> getBorrowedBooksForUser(int userId);
    std::vector<Book> getBorrowedBooksForUser(int userId);
    std::vector<Book> getAllBooks();
    int getUserId(QString username);
    int getBookId(QString title);
    bool checkDateOfBorrowing(QDate date);
    bool tooManyUnreturned(QString userId);
    QDate extendDate(QDate date);

    int countBooks();
    int countBorrowedBooks(QString username);
    bool validUsername(QString username);
};

#endif // DATABASE_MANAGER_H
