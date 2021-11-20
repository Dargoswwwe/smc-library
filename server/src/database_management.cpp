#include "database_management.hpp"


database_management::database_management()
{
    database = QSqlDatabase::addDatabase("QSQLITE");

    databaseDir = QDir::homePath().toStdString();
#ifdef Q_OS_LINUX
    databaseDir += "/.local/share/smc-library/";
#elif defined(Q_OS_WINDOWS)
    databaseDir += "\\Documents\\smc-library\\";
#else
    databaseDir = "./";
#endif

    if (!QDir(databaseDir.c_str()).exists()) QDir().mkpath(databaseDir.c_str());

    if (!QDir(databaseDir.c_str()).exists()) databaseDir = "./";
    database.setDatabaseName((databaseDir + "library.sqlite").c_str());

    if (!database.open()) {
        qWarning() << "Error creating database file. Using in-memory database.\n" << database.lastError();
        database.setDatabaseName(":memory:");
        databaseDir="";
    }

    // Check if data base is empty and create book table into it
    if (!database.contains(QLatin1String("Books"))) {
        QSqlQuery query;
        query.exec("create table Books "
                   "(id integer primary key, "
                   "title varchar(50), "
                   "authors varchar(100), "
                   "language varchar(20), "
                   "original_publication_year integer, "
                   "avarage_rating real, "
                   "ratings_count integer, "
                   "isbn varchar(50), "
                   "image_url varchar(100))");
    }

    // Check if data base is empty and create user table into it
    if (!database.contains(QLatin1String("Users"))) {
        QSqlQuery query;
        query.exec("create table Users "
                   "(id integer primary key, "
                   "username varchar(50), "
                   "password varchar(50))");
    }
}

void database_management:: deleteUsersTable()
{
    QSqlQuery query;
    query.exec("Drop table Users ");
}

void database_management:: deleteBookTable()
{
    QSqlQuery query;
    query.exec("Drop table Books ");
}

void database_management:: deleteBook(QString title)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Books WHERE title =  (:title)");
    query.bindValue(":title", title);

    if (!query.exec()) qDebug() << "Error deleting a book." << query.lastError().text();
}

void database_management:: deleteUser(QString username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Users WHERE username =  (:username)");
    query.bindValue(":username", username);

    if (!query.exec()) qDebug() << "Error deleting a user." << query.lastError().text();
}

void database_management:: addValuesIntoBookTable(int id, QString title, QString authors, QString language, int original_publication_year,
                            float avarage_rating, int ratings_count, QString isbn, QString image_url)
{
    QSqlQuery query;

    query.prepare("INSERT into Books ("
                  "id, "
                  "title, "
                  "authors, "
                  "language, "
                  "original_publication_year, "
                  "avarage_rating,"
                  "ratings_count, "
                  "isbn, "
                  "image_url)"
                  "VALUES (?,?,?,?,?,?,?,?,?);");

    query.addBindValue(id);
    query.addBindValue(title);
    query.addBindValue(authors);
    query.addBindValue(language);
    query.addBindValue(original_publication_year);
    query.addBindValue(avarage_rating);
    query.addBindValue(ratings_count);
    query.addBindValue(isbn);
    query.addBindValue(image_url);

    if (!query.exec()) qDebug() << "Error adding value.";
}

void database_management:: addValuesIntoUsersTable(int id, QString username, QString password)
{
    QSqlQuery query;

    query.prepare("INSERT into Users ("
                  "id, "
                  "username, "
                  "password)"

                  "VALUES (?,?,?);");

    query.addBindValue(id);
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) qDebug() << "Error adding value.";
}

void database_management:: changeUserPassword(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET password = (:password) WHERE username = (:username)");
    query.bindValue(":password", password);
    query.bindValue(":username", username);

    query.exec();
}

void database_management:: changeUsername(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET  username = (:username) WHERE password = (:password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    query.exec();
}

void database_management:: insertBooksIntoDataBase()
{
    QFile inputFile((databaseDir+"books.csv").c_str());
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
        qDebug() << "Error";

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {

        QStringList word=line.split(',');
        addValuesIntoBookTable(word[0].toInt(), word.at(1), word.at(2), word.at(3),word.at(4).toInt(),
                word.at(5).toFloat(), word.at(6).toInt(), word.at(7), word.at(8));

    };
}
