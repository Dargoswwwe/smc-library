#include "server.hpp"

Server::Server(QObject* parent)
{
    this->setParent(parent);

    initServer();
}

void Server::initServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 4200)) {
        qWarning() << "Cannot start server: " << tcpServer->serverError();
        throw tcpServer->serverError();
    } else qDebug() << "Started library server on port" << tcpServer->serverPort();
}

void Server::initDatabase()
{
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

    database = QSqlDatabase::addDatabase("QSQLITE");

    std::string databaseDir = QDir::homePath().toStdString();
#ifdef Q_OS_LINUX
    databaseDir += "/.local/share/smc-library/";
#elif Q_OS_CYGWIN
    databaseDir += "/Documents\\smc-library\\";
#else
    databaseDir = "./";
#endif

    if (!QDir(databaseDir.c_str()).exists()) QDir().mkdir(databaseDir.c_str());

    if (!QDir(databaseDir.c_str()).exists()) databaseDir = "./";
    database.setDatabaseName((databaseDir + "library.sqlite").c_str());

    if (!database.open()) {
        qWarning() << "Error creating database file. Using in-memory database.\n" << database.lastError();
        database.setDatabaseName(":memory:");
    }
}

void Server::newConnection() { qDebug() << "Hello, client!"; }
