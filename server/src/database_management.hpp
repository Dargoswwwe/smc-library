#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>
#include <QJsonObject>


class database_management
{
private:
    QSqlDatabase database;
public:
    database_management();
    void deleteUsersTable();
    void deleteBookTable();
    void deleteBook(QString title);
    void deleteUser(QString username);
    void addValuesIntoBookTable(int id, QString title, QString authors, QString language, int original_publication_year,
                                float avarage_rating, int ratings_count, QString isbn, QString image_url);
    void addValuesIntoUsersTable(int id, QString username, QString password);
    void changeUserPassword(QString username, QString password);
    void changeUsername(QString username, QString password);
};

#endif // DATABASE_MANAGEMENT_H
