#include "server.hpp"
#include "json.hpp"

using nlohmann::json;

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

    if(database.countBooks()<10000)
        database.insertBooksIntoDataBase();
    else
        qDebug()<<"The library already has all the books.";

    Library library;
    library.setAllBooks(database.createBooksArray());

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

    json json = R"({ "type" : "testMessage", "data" : "hello, client" })"_json;

    sendData(clientSocket, json);

    QObject::connect(clientSocket, &QIODevice::readyRead, this, &Server::receiveData);
}

void Server::sendData(QTcpSocket* clientSocket, const json& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString::fromStdString(data.dump());
    clientSocket->write(block);
}

void Server::receiveData()
{
    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());

    QDataStream* inStream = connections[clientSocket].second;

    inStream->startTransaction();
    QString messageString;
    *inStream >> messageString;

    try {
        json message = json::parse(messageString.toStdString());

        qDebug() << message.dump().c_str();

        if (message["type"] == "register") {
            registerUser(message["data"]["username"], message["data"]["password"], clientSocket);
        }
    } catch (const nlohmann::detail::parse_error& e) {
        qWarning() << e.what();
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << e.what();
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

void Server::loginUser() { }

void Server::registerUser(const std::string& name, const std::string& password, QTcpSocket* clientSocket)
{
    // std::optional<User> oldUser =database.getUser(name);
    // if(oldUser.has_value()){
    //    sendData(clientSocket,
    //        R"({"type": "register", "response": "error"})"_json);
    //     }
    //   else{

    sendData(clientSocket, R"({"type": "register", "response": "success"})"_json);

    database.addValuesIntoUsersTable(name.c_str(), password.c_str());
    //   }
}
