#include <QCoreApplication>

#include "server.hpp"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Library server");
    Server server;

   return app.exec();
}
