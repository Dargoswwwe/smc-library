#include "database_manager.hpp"

#include "user.hpp"

#include <unordered_map>

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

    if (!QDir(databaseDir.c_str()).exists())
        QDir().mkpath(databaseDir.c_str());

    if (!QDir(databaseDir.c_str()).exists())
        databaseDir = "./";
    database.setDatabaseName((databaseDir + "library.sqlite").c_str());

    if (!database.open()) {
        qWarning() << "Error creating database file. Using in-memory database.\n"
                   << database.lastError();
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
                   "average_rating real, "
                   "ratings_count integer, "
                   "isbn varchar(50), "
                   "image_url varchar(100),"
                   "available_books integer)");
    }

    // Check if data base is empty and create user table into it
    if (!database.contains(QLatin1String("Users"))) {
        QSqlQuery query;
        query.exec("create table Users "
                   "(user_id integer primary key,"
                   "username varchar(50), "
                   "password varchar(50))");
    }

    // Check if data base is empty and create UsersBooks table into it
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

void DatabaseManager::deleteRowFromUsersBooksTable(int userId, int bookId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM UsersBooks WHERE user_id =  (:user_id) AND book_id = (:book_id)");
    query.bindValue(":user_id", userId);
    query.bindValue(":book_id", bookId);

    if (!query.exec())
        qDebug() << "Error deleting row." << query.lastError().text();
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

    if (!query.exec())
        qDebug() << "Error deleting a book." << query.lastError().text();
}

void DatabaseManager::deleteUser(QString username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Users WHERE username =  (:username)");
    query.bindValue(":username", username);

    if (!query.exec())
        qDebug() << "Error deleting a user." << query.lastError().text();
}

bool DatabaseManager::addValuesIntoBookTable(int id, QString title, QString authors, QString language,
    int original_publication_year, float average_rating, int ratings_count, QString isbn, QString image_url,
    int available_books)
{
    QSqlQuery query;

    query.prepare("INSERT OR REPLACE INTO Books ("
                  "book_id, "
                  "title, "
                  "authors, "
                  "language, "
                  "original_publication_year, "
                  "average_rating,"
                  "ratings_count, "
                  "isbn, "
                  "image_url, "
                  "available_books)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?);");

    query.addBindValue(id);
    query.addBindValue(title);
    query.addBindValue(authors);
    query.addBindValue(language);
    query.addBindValue(original_publication_year);
    query.addBindValue(average_rating);
    query.addBindValue(ratings_count);
    query.addBindValue(isbn);
    query.addBindValue(image_url);
    query.addBindValue(available_books);

    if (!query.exec()) {
        qDebug() << "Error adding value: " << query.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::addValuesIntoUsersTable(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("INSERT into Users ("
                  "username, "
                  "password)"

                  "VALUES (?,?);");

    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Error adding user.";
        return false;
    }
    return true;
}

bool DatabaseManager::addValuesIntoUsersBooksTable(int userId, int bookId)
{
    QSqlQuery query;
    QDate borrowingDate;

    query.prepare("INSERT into UsersBooks ("
                  "user_id, "
                  "book_id,"
                  "date_of_borrowing)"

                  "VALUES (?,?,?);");

    borrowingDate = QDate::currentDate();

    query.addBindValue(userId);
    query.addBindValue(bookId);
    query.addBindValue(borrowingDate);

    if (!query.exec()) {
        qDebug() << "Error binding book with user.";
        return false;
    }
    return true;
}

void DatabaseManager::changeUserPassword(QString username, QString password)
{
    QSqlQuery query;
    User user;
    user.setUsername(username.toStdString());
    user.setPassword(password.toStdString());
    query.prepare("UPDATE Users SET password = (:password) WHERE username = (:username)");
    query.bindValue(":password", user.getPassword().c_str());
    query.bindValue(":username", user.getUsername().c_str());

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
    int valuesAdded = 0;
    qDebug() << "Importing books from the CSV file into the database...";
    QFile inputFile((databaseDir + "books.csv").c_str());
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
        qDebug() << "Error";

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {

        QStringList word = line.split(',');
        valuesAdded += addValuesIntoBookTable(word[0].toInt(), word.at(1), word.at(2), word.at(3), word.at(4).toInt(),
            word.at(5).toFloat(), word.at(6).toInt(), word.at(7), word.at(8), word.at(9).toInt());
    };

    qDebug() << "Done. Imported" << valuesAdded;
}

void DatabaseManager::getUsersForBook(int book_id)
{
    QSqlQuery query;
    query.prepare("SELECT u.username, ub.date_of_borrowing FROM UsersBooks ub INNER JOIN Users u on "
                  "ub.user_id=u.user_id WHERE ub.book_id =  (:book_id) ");

    query.bindValue(":book_id", book_id);

    if (!query.exec())
        qDebug() << "Error selecting users for this book!" << query.lastError().text();
}

int DatabaseManager::getUserId(QString username)
{
    QSqlQuery query;
    int id;
    query.prepare("SELECT * FROM Users u WHERE u.username =  (:username) ");

    query.bindValue(":username", username);

    if (!query.exec())
        qDebug() << "Error getting id for this user!" << query.lastError().text();
    query.next();

    id = query.value(0).toInt();

    return id;
}

int DatabaseManager::getBookId(QString title)
{
    QSqlQuery query;
    int id;
    query.prepare("SELECT * FROM Books b WHERE b.title =  (:title) ");

    query.bindValue(":title", title);

    if (!query.exec())
        qDebug() << "Error getting id for this book!" << query.lastError().text();
    query.next();

    id = query.value(0).toInt();

    return id;
}

std::vector<Book> DatabaseManager::getBorrowedBooksForUser(int userId)
{
    QSqlQuery query;
    query.prepare(
        "SELECT b.title, b.authors, b.language, b.original_publication_year, b.average_rating,"
        " b.ratings_count, b.isbn, ub.date_of_borrowing FROM UsersBooks ub INNER JOIN Books b on ub.book_id=b.book_id "
        "WHERE ub.user_id = (:user_id)");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "Error selecting borrowed books for this user!" << query.lastError().text();
        return std::vector<Book>();
    }

    std::vector<std::string> fieldNames = { "title", "authors", "language", "original_publication_year",
        "average_rating", "ratings_count", "isbn", "date_of_borrowing" };
    std::unordered_map<std::string, int> valueIndex;

    for (auto& fieldName : fieldNames)
        valueIndex[fieldName] = query.record().indexOf(fieldName.c_str());

    std::vector<Book> books;

    while (query.next()) {
        Book book;
        book.setTitle(query.value(valueIndex["title"]).toString().toStdString());

        std::stringstream authorsStream(query.value(valueIndex["authors"]).toString().toStdString());

        std::vector<std::string> authors;
        std::string author;

        while (std::getline(authorsStream, author, ';'))
            authors.push_back(author);
        book.setAuthors(authors);

        book.setLanguage(query.value(valueIndex["language"]).toString().toStdString());
        book.setOriginalPublication(query.value(valueIndex["language"]).toInt());
        book.setAverageRating(query.value(valueIndex["average_rating"]).toFloat());
        book.setRatingsCount(query.value(valueIndex["average_rating"]).toInt());
        book.setIsbn(query.value(valueIndex["isbn"]).toString().toStdString());
        book.setUrl(query.value(valueIndex["image_url"]).toString().toStdString());

        books.push_back(book);
    }

    return books;
}

// std::vector<std::pair<Book, std::string>> DatabaseManager::getBorrowedBooksForUser(int userId)
//{
//     QSqlQuery query;
//     query.prepare(
//                 "SELECT b.title, b.authors, b.language, b.original_publication_year, b.average_rating,"
//                 " b.ratings_count, b.isbn, ub.date_of_borrowing FROM UsersBooks ub INNER JOIN Books b on
//                 ub.book_id=b.book_id " "WHERE ub.user_id = (:user_id)");

//    query.bindValue(":user_id", userId);

//    if (!query.exec()) {
//        qDebug() << "Error selecting borrowed books for this user!" << query.lastError().text();
//        return std::vector<std::pair<Book, std::string>>();
//    }

//    std::vector<std::string> fieldNames = { "title", "authors", "language", "original_publication_year",
//                                            "average_rating", "ratings_count", "isbn", "date_of_borrowing" };
//    std::unordered_map<std::string, int> valueIndex;

//    for (auto& fieldName : fieldNames)
//        valueIndex[fieldName] = query.record().indexOf(fieldName.c_str());

//    std::vector<std::pair<Book, std::string>> books;

//    while (query.next()) {
//        Book book;
//        std::string date;

//        book.setTitle(query.value(valueIndex["title"]).toString().toStdString());

//        std::stringstream authorsStream(query.value(valueIndex["authors"]).toString().toStdString());

//        std::vector<std::string> authors;
//        std::string author;

//        while (std::getline(authorsStream, author, ';'))
//            authors.push_back(author);
//        book.setAuthors(authors);

//        book.setLanguage(query.value(valueIndex["language"]).toString().toStdString());
//        book.setOriginalPublication(query.value(valueIndex["language"]).toInt());
//        book.setAverageRating(query.value(valueIndex["average_rating"]).toFloat());
//        book.setRatingsCount(query.value(valueIndex["average_rating"]).toInt());
//        book.setIsbn(query.value(valueIndex["isbn"]).toString().toStdString());
//        book.setUrl(query.value(valueIndex["image_url"]).toString().toStdString());

//        date=query.value(valueIndex["date_of_borrowing"]).toString().toStdString();
//        books.push_back(std::make_pair(book, date));
//    }

//    return books;
//}
std::vector<Book> DatabaseManager::getAllBooks()
{
    QSqlQuery query;
    query.prepare("SELECT b.title, b.authors, b.language, b.original_publication_year, b.average_rating, "
                  "b.ratings_count, b.isbn FROM Books b");

    if (!query.exec()) {
        qDebug() << query.executedQuery();
        qDebug() << "Error selecting all books" << query.lastError();
        return std::vector<Book>();
    }

    std::vector<std::string> fieldNames = {
        "title",
        "authors",
        "language",
        "original_publication_year",
        "average_rating",
        "ratings_count",
        "isbn",
    };

    std::unordered_map<std::string, int> valueIndex;

    for (auto& fieldName : fieldNames)
        valueIndex[fieldName] = query.record().indexOf(fieldName.c_str());

    std::vector<Book> books;

    while (query.next()) {
        Book book;
        book.setTitle(query.value(valueIndex["title"]).toString().toStdString());

        std::stringstream authorsStream(query.value(valueIndex["authors"]).toString().toStdString());

        std::vector<std::string> authors;
        std::string author;

        while (std::getline(authorsStream, author, ';'))
            authors.push_back(author);
        book.setAuthors(authors);

        book.setLanguage(query.value(valueIndex["language"]).toString().toStdString());
        book.setOriginalPublication(query.value(valueIndex["original_publication_year"]).toUInt());
        book.setAverageRating(query.value(valueIndex["average_rating"]).toFloat());
        book.setRatingsCount(query.value(valueIndex["ratings_count"]).toUInt());
        book.setIsbn(query.value(valueIndex["isbn"]).toString().toStdString());
        book.setUrl(query.value(valueIndex["image_url"]).toString().toStdString());

        books.push_back(book);
    }

    return books;
}

int DatabaseManager::countBooks()
{
    QSqlQuery query;
    int count;
    query.exec("SELECT COUNT (*) FROM Books");
    query.next();
    count = query.value(0).toInt();
    return count;
}

bool DatabaseManager::validUsername(QString username)
{
    QSqlQuery query;
    QString name;
    query.prepare("SELECT * FROM Users u WHERE u.username=(:username)");
    query.bindValue(":username", username);
    query.exec();
    query.next();

    name = query.value(0).toString();

    if (name == nullptr) {
        return false;
    }

    return true;
}

bool DatabaseManager::uniquePassword(QString password)
{
    QSqlQuery query;
    QString pass;
    query.prepare("SELECT * FROM Users u WHERE u.password=(:password)");
    query.bindValue(":password", password);
    query.exec();
    query.next();

    pass = query.value(0).toString();

    if (pass == nullptr) {
        return false;
    }

    return true;
}

bool DatabaseManager::validPassword(QString username, QString password)
{
    QSqlQuery query;
    QString pass;

    query.prepare("SELECT password FROM Users u WHERE u.username=(:username)");
    query.bindValue(":username", username);
    query.exec();
    query.next();
    pass = query.value(0).toString();

    if (pass != password) {
        return false;
    }

    return true;
}

std::vector<std::string> separator(std::string str, std::regex delimitator)
{
    std::sregex_token_iterator iterator { str.begin(), str.end(), delimitator, -1 };
    std::vector<std::string> elemSeparate { iterator, {} };
    return elemSeparate;
}

std::vector<Book> DatabaseManager::createBooksArray()
{
    std::vector<Book> booksArray;

    QFile inputFile((databaseDir + "books.csv").c_str());
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
        qDebug() << "Error";

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {
        Book newBook;
        QStringList word = line.split(',');

        newBook.setTitle(word.at(1).toStdString());

        std::vector<std::string> authors;
        std::regex divider(";");
        std::vector<std::string> authorsToken = separator(word.at(2).toStdString(), divider);
        for (std::string& author : authorsToken) {
            authors.push_back(author);
        }

        newBook.setAuthors(authors);
        newBook.setLanguage(word.at(3).toStdString());
        newBook.setOriginalPublication(word.at(4).toInt());
        newBook.setAverageRating(word.at(5).toFloat());
        newBook.setRatingsCount(word.at(6).toInt());
        newBook.setIsbn(word.at(7).toStdString());
        newBook.setUrl(word.at(8).toStdString());
        newBook.setExemplarsAvailable(word.at(9).toInt());
        booksArray.push_back(newBook);
    };

    return booksArray;
}

bool DatabaseManager::isBookAvailable(int book_id)
{
    int available;

    QSqlQuery query;
    query.prepare("SELECT available_books FROM Books b WHERE b.book_id=(:book_id)");
    query.bindValue(":book_id", book_id);
    query.exec();
    query.next();
    available = query.value(0).toInt();

    if (available > 0)
        return true;
    return false;
}

void DatabaseManager::updateAvailableBook(QString title, int available_books)
{
    QSqlQuery query;

    query.prepare("UPDATE Books SET available_books = (:available_books) WHERE title = (:title)");
    query.bindValue(":available_books", available_books);
    query.bindValue(":title", title);

    query.exec();
}

void DatabaseManager::decreaseAvailableBook(QString title)
{
    int available;

    QSqlQuery query;
    query.prepare("SELECT available_books FROM Books b WHERE b.title=(:title)");
    query.bindValue(":title", title);
    query.exec();
    query.next();
    available = query.value(0).toInt();
    available--;
    QSqlQuery query2;
    query2.prepare("UPDATE Books SET available_books = (:available_books) WHERE title = (:title)");
    query2.bindValue(":available_books", available);
    query2.bindValue(":title", title);

    query2.exec();
}

void DatabaseManager::increaseAvailableBook(QString title)
{
    int available;

    QSqlQuery query;
    query.prepare("SELECT available_books FROM Books b WHERE b.title=(:title)");
    query.bindValue(":title", title);
    query.exec();
    query.next();
    available = query.value(0).toInt();
    available++;

    QSqlQuery query2;
    query2.prepare("UPDATE Books SET available_books = (:available_books) WHERE title = (:title)");
    query2.bindValue(":available_books", available);
    query2.bindValue(":title", title);

    query2.exec();
}
