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

    qDebug() << tcpServer->serverPort();
    tcpServer->setProperty("port", 4200);
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::newConnection() { qDebug() << "Hello, client!"; }
