#include "server.hpp"

Server::Server(QObject* parent)
{
    this->setParent(parent);
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 4200)) {
        qWarning() << "Cannot start server!\n";
        return;
    }

    qDebug() << tcpServer->serverPort();
    tcpServer->setProperty("port", 4200);
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::newConnection() { qDebug() << "Hello, client!"; }
