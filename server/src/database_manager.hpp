#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>

class DatabaseManager {
private:
    QSqlDatabase database;
    std::string databaseDir;

public:
    DatabaseManager();

    void deleteUsersTable();
    void deleteBookTable();
    void deleteBook(QString title);
    void deleteUser(QString username);
    void addValuesIntoBookTable(int id, QString title, QString authors, QString language, int original_publication_year,
        float avarage_rating, int ratings_count, QString isbn, QString image_url);
    void addValuesIntoUsersTable(QString username, QString password);
    void changeUserPassword(QString username, QString password);
    void changeUsername(QString username, QString password);
    void insertBooksIntoDataBase();
};

#endif // DATABASE_MANAGER_H
