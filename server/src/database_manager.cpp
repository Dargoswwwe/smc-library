#include "database_manager.hpp"

DatabaseManager::DatabaseManager()
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
        databaseDir = "";
    }

    QSqlQuery query(database);
    query.exec("PRAGMA foreign_keys = ON;");

    // Check if data base is empty and create book table into it
    if (!database.contains(QLatin1String("Books"))) {
        QSqlQuery query;
        query.exec("create table Books "
                   "(book_id integer primary key, "
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
                   "(user_id integer primary key,"
                   "username varchar(50), "
                   "password varchar(50))");
    }

    //Check if data base is empty and create UsersBooks table into it
    if (!database.contains(QLatin1String("UsersBooks"))) {
        QSqlQuery query;
        query.exec("create table UsersBooks "
                   "(user_id integer,"
                   "book_id integer,"
                   "date_of_borrowing text,"
                   "FOREIGN KEY(user_id) REFERENCES Users(user_id)ON DELETE CASCADE,"
                   "FOREIGN KEY(book_id) REFERENCES Books(book_id)ON DELETE CASCADE,"
                   "UNIQUE (user_id, book_id))");
    }

}

void DatabaseManager::deleteRowFromUsersBooksTable(int user_id, int book_id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM UsersBooks WHERE user_id =  (:user_id) AND book_id = (:book_id)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":book_id", book_id);

    if (!query.exec()) qDebug() << "Error deleting row." << query.lastError().text();
}

void DatabaseManager::deleteUsersTable()
{
    QSqlQuery query;
    query.exec("Drop table Users ");
}

void DatabaseManager::deleteUsersBooksTable()
{
    QSqlQuery query;
    query.exec("Drop table UsersBooks ");
}

void DatabaseManager::deleteBookTable()
{
    QSqlQuery query;
    query.exec("Drop table Books ");
}

void DatabaseManager::deleteBook(QString title)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Books WHERE title =  (:title)");
    query.bindValue(":title", title);

    if (!query.exec()) qDebug() << "Error deleting a book." << query.lastError().text();
}

void DatabaseManager::deleteUser(QString username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Users WHERE username =  (:username)");
    query.bindValue(":username", username);

    if (!query.exec()) qDebug() << "Error deleting a user." << query.lastError().text();
}



void DatabaseManager::addValuesIntoBookTable(int id, QString title, QString authors, QString language,
                                             int original_publication_year, float avarage_rating, int ratings_count, QString isbn,
                                             QString image_url)
{
    QSqlQuery query;

    query.prepare("INSERT into Books ("
                  "book_id, "
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

    if (!query.exec()) qDebug() << "Error adding value: " << query.lastError();
}

void DatabaseManager::addValuesIntoUsersTable(QString username, QString password)
{

    QSqlQuery query;

    query.prepare("INSERT into Users ("
                  "username, "
                  "password)"

                  "VALUES (?,?);");

    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) qDebug() << "Error adding user.";
}

void DatabaseManager::addValuesIntoUsersBooksTable(int user_id, int book_id)
{

    QSqlQuery query;
    QDate date_of_borrowing;

    query.prepare("INSERT into UsersBooks ("
                  "user_id, "
                  "book_id,"
                  "date_of_borrowing)"

                  "VALUES (?,?,?);");

    date_of_borrowing=QDate::currentDate();

    query.addBindValue(user_id);
    query.addBindValue(book_id);
    query.addBindValue(date_of_borrowing);

    if (!query.exec()) qDebug() << "Error binding book with user.";
}


void DatabaseManager::changeUserPassword(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET password = (:password) WHERE username = (:username)");
    query.bindValue(":password", password);
    query.bindValue(":username", username);

    query.exec();
}

void DatabaseManager::changeUsername(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET  username = (:username) WHERE password = (:password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    query.exec();
}


void DatabaseManager::insertBooksIntoDataBase()
{
    QFile inputFile((databaseDir + "books.csv").c_str());
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen()) qDebug() << "Error";

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {

        QStringList word = line.split(',');
        addValuesIntoBookTable(word[0].toInt(), word.at(1), word.at(2), word.at(3), word.at(4).toInt(),
                word.at(5).toFloat(), word.at(6).toInt(), word.at(7), word.at(8));
    };
}

void DatabaseManager:: displayUsersForBook(int book_id)
{
    QSqlQuery query;
    query.prepare("SELECT u.username, ub.date_of_borrowing FROM UsersBooks ub INNER JOIN Users u on ub.user_id=u.user_id WHERE ub.book_id =  (:book_id) ");

    query.bindValue(":book_id", book_id);

    if (!query.exec()) qDebug() << "Error displaying users for this book." << query.lastError().text();
}

void DatabaseManager:: displayBooksForUser(int user_id)
{
    QSqlQuery query;
    query.prepare("SELECT b.title, b.authors, b.language,b.original_publication_year, b.avarage_rating,"
                  " b.ratings_count, b.isbn, ub.date_of_borrowing FROM UsersBooks ub INNER JOIN Books b on ub.book_id=b.book_id "
                  "WHERE ub.user_id =  (:user_id) ");

    query.bindValue(":user_id", user_id);

    if (!query.exec()) qDebug() << "Error displaying books for this user." << query.lastError().text();
}

int DatabaseManager::countBooks()
{
    QSqlQuery query;
    int count;
    query.exec("SELECT COUNT (*) FROM Books");
    query.next();
    count=query.value(0).toInt();
    return count;
}




