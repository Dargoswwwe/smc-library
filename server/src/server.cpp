#include "server.hpp"
#include "json.hpp"
using namespace nlohmann;

Server::Server(QObject* parent)
{
    this->setParent(parent);

    initServer();
}

Server::~Server()
{
    for (auto [clientSocket, temp] : connections)
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

    database.insertBooksIntoDataBase();
}

void Server::newConnection()
{

    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    QDataStream* inStream = new QDataStream(clientSocket);
    inStream->setVersion(QDataStream::Qt_5_15);

    connect(clientSocket, &QAbstractSocket::disconnected, clientSocket, &QObject::deleteLater);

    connect(clientSocket, &QAbstractSocket::disconnected, this, &Server::disconnect);
    connections[clientSocket] = { std::nullopt, inStream };

    qDebug() << "New client connected: " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
    qDebug() << "Connections: " << connections.size();

    QJsonObject json { { "header", "testMessage" }, { "messge", "hello, client" } };
    sendData(clientSocket, json);

    QObject::connect(clientSocket, &QIODevice::readyRead, this, &Server::receiveData);
}

void Server::sendData(QTcpSocket* clientSocket, const QJsonObject& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << data;
    clientSocket->write(block);
}

void Server::receiveData()
{

    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());

    QDataStream* inStream = connections[clientSocket].second;

    inStream->startTransaction();
    json data;
    QString message;
    *inStream >> message;

    data=json::parse(message);

    if(data["messageType"]=="register")
    {
       registerUser(data["data"]["username"],data["data"]["password"]);
    }
    if (!inStream->commitTransaction()) return;
}

void Server::disconnect()
{
    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());
    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
    delete connections[clientSocket].second;
    connections.erase(clientSocket);
    qDebug() << "Connections: " << connections.size();
}

void Server::loginUser()
{

}


