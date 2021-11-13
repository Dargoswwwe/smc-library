#include "server.hpp"

Server::Server(QObject* parent)
{
    this->setParent(parent);

    initServer();
    initDatabase();
}

void Server::initServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 4200)) {
        qWarning() << "Cannot start server: " << tcpServer->serverError();
        throw tcpServer->serverError();
    } else qDebug() << "Started library server on port" << tcpServer->serverPort();
}

void deleteUsersTable()
{
    QSqlQuery query;
    query.exec("Drop table Users ");
}

void deleteBookTable()
{
    QSqlQuery query;
    query.exec("Drop table Books ");
}

void deleteBook(QString title)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Books WHERE title =  (:title)");
    query.bindValue(":title",title);

    if (!query.exec())
        qDebug() << "Error deleting a book." << query.lastError().text();
}

void Server::initDatabase()
{
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

    database = QSqlDatabase::addDatabase("QSQLITE");

    std::string databaseDir = QDir::homePath().toStdString();
#ifdef Q_OS_LINUX
    databaseDir += "/.local/share/smc-library/";
    qDebug() << "LINUX";
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
    }


    //Check if data base is empty and create book table into it
    if(!database.contains(QLatin1String("Books")))
    { QSqlQuery query;
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


    //Check if data base is empty and create user table into it
    if(!database.contains(QLatin1String("Users")))
    { QSqlQuery query;
        query.exec("create table Users "
                   "(id integer primary key, "
                   "username varchar(50), "
                   "password varchar(50))");
    }


}



void Server::newConnection() { qDebug() << "Hello, client!"; }
