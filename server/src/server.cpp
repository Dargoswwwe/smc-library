#include "server.hpp"

Server::Server(QObject* parent)
{
    this->setParent(parent);

    initServer();

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

     connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
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
