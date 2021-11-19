#include "server.hpp"

Server::Server(QObject* parent)
{
    this->setParent(parent);

    initServer();
    initDatabase();
}

Server::~Server()
{
    for (auto [clientSocket, user] : connections)
        clientSocket->disconnectFromHost();
}

void Server::initServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 4200)) {
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
    query.bindValue(":title", title);

    if (!query.exec()) qDebug() << "Error deleting a book." << query.lastError().text();
}

void deleteUser(QString username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Users WHERE username =  (:username)");
    query.bindValue(":username", username);

    if (!query.exec()) qDebug() << "Error deleting a user." << query.lastError().text();
}

void addValuesIntoBookTable(int id, QString title, QString authors, QString language, int original_publication_year,
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

void addValuesIntoUsersTable(int id, QString username, QString password)
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

void changeUserPassword(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET password = (:password) WHERE username = (:username)");
    query.bindValue(":password", password);
    query.bindValue(":username", username);

    query.exec();
}

void changeUsername(QString username, QString password)
{
    QSqlQuery query;

    query.prepare("UPDATE Users SET  username = (:username) WHERE password = (:password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    query.exec();
}

void Server::initDatabase()
{
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

    database = QSqlDatabase::addDatabase("QSQLITE");

    std::string databaseDir = QDir::homePath().toStdString();
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

void Server::newConnection()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);

    connect(clientSocket, &QAbstractSocket::disconnected, this, &Server::disconnect);
    connections[clientSocket] = std::nullopt;

    qDebug() << "New client connected: " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
    qDebug() << "Connections: " << connections.size();

    QJsonObject json { { "header", "testMessage" }, { "messge", "hello, client" } };
    sendData(clientSocket, json);
}

void Server::sendData(QTcpSocket* clientSocket, const QJsonObject& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << data;
    clientSocket->write(block);
}

void Server::disconnect()
{
    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());
    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
    connections.erase(clientSocket);
    qDebug() << "Connections: " << connections.size();
}
