#include <QCoreApplication>

#include "server.hpp"
#include"database_management.hpp"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Library server");
    Server server;
    database_management db;


    db.insertBooksIntoDataBase();

    return app.exec();
}
