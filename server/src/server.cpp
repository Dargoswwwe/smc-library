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

    if (database.countBooks() < 10000) database.insertBooksIntoDataBase();
    else qDebug() << "The library already has all the books.";

    library.setAllBooks(database.getAllBooks());
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

        handleMessage(clientSocket, message["type"], message["data"]);
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

void Server::loginUser(const std::string& name, const std::string& password, QTcpSocket* clientSocket)
{
    if (!database.validUsername(name.c_str())) {
        sendData(clientSocket, R"({"type": "login", "data": "NameError"})"_json);
    } else {
        if (database.validPassword(name.c_str(), password.c_str())) {
            User newUser(name, password);
            newUser.setPassword(password);
            connections[clientSocket].first = newUser;

            sendData(clientSocket, R"({"type": "login", "data": "Success"})"_json);
        } else {
            sendData(clientSocket, R"({"type": "login", "data": "PasswordError"})"_json);
        }
    }
}

void Server::registerUser(const std::string& name, const std::string& password, const ::std::string& confirmpassword,
    QTcpSocket* clientSocket)
{
    if (database.validUsername(name.c_str())) {
        sendData(clientSocket, R"({"type": "register", "data": "UsernameAlreadyTaken"})"_json);
    } else if (password != confirmpassword) {
        sendData(clientSocket, R"({"type": "register", "data": "NotMatchingPasswords"})"_json);
    } else if (database.uniquePassword(password.c_str())) {
        sendData(clientSocket, R"({"type": "register", "data": "PasswordAlreadyTaken"})"_json);
    } else {
        database.addValuesIntoUsersTable(name.c_str(), password.c_str());
        sendData(clientSocket, R"({"type": "register", "data": "Success"})"_json);
    }
}

void Server::changeUsername(const std::string& name, const std::string& password, QTcpSocket* clientSocket)
{
    if (database.validUsername(name.c_str())) {
        sendData(clientSocket, R"({"type": "changeUsername", "data": "AlreadyTaken"})"_json);
    } else {
        database.changeUsername(name.c_str(), password.c_str());

        sendData(clientSocket, R"({"type": "changeUsername", "data": "Success"})"_json);
    }
}

void Server::changePassword(const std::string& name, const std::string& password, const std::string& oldpassword,
    const std::string& newpassword, const std::string& confirmpassword, QTcpSocket* clientSocket)
{
    if (oldpassword != password) {
        sendData(clientSocket, R"({"type": "changePassword", "data": "IncorrectOldPassword"})"_json);
    } else if (password == newpassword) {
        sendData(clientSocket, R"({"type": "changePassword", "data": "SameWithOldPassword"})"_json);
    } else if (newpassword != confirmpassword) {
        sendData(clientSocket, R"({"type": "changePassword", "data": "NotMatchingPasswords"})"_json);
    } else {
        database.changeUserPassword(name.c_str(), newpassword.c_str());
        sendData(clientSocket, R"({"type": "changePassword", "data": "Success"})"_json);
    }
}

void Server::logout(QTcpSocket* clientSocket)
{
    sendData(clientSocket, R"({"type": "logout", "data": "Success"})"_json);
}

void Server::deleteAccount(const std::string& name, QTcpSocket* clientSocket)
{

    database.deleteUser(name.c_str());
    sendData(clientSocket, R"({"type": "deleteAccount", "data": "Success"})"_json);
}

void Server::sendAllBooks(QTcpSocket* clientSocket)
{
    auto books = library.getAllBooks();
    size_t messageSize = 50;

    for (size_t i = 0; i < books.size() / messageSize; ++i) {
        json message;
        message["type"] = MessageType::GET_ALL_BOOKS;
        message["data"] = json::array({});
        for (size_t j = 0; j < messageSize; j++)
            message["data"][j] = books[i + j];

        sendData(clientSocket, message);
    }

    for (size_t i = books.size() - books.size() % messageSize; i < books.size(); ++i) {
        json message;
        message["type"] = MessageType::GET_ALL_BOOKS;
        message["data"] = json::array({});
        for (size_t j = 0; j < messageSize; j++)
            message["data"][j] = books[i + j];

        sendData(clientSocket, message);
    }
    sendData(clientSocket, R"({"type": "finished", "data": ""})"_json);
}

void Server::handleMessage(QTcpSocket* clientSocket, MessageType messageType, const json& messageData)
{
    switch (messageType) {
    case MessageType::REGISTER:
        try {
            registerUser(
                messageData["username"], messageData["password"], messageData["confirmpassword"], clientSocket);
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::LOGIN:
        loginUser(messageData["username"], messageData["password"], clientSocket);
        break;

    case MessageType::CHANGE_USERNAME:
        changeUsername(messageData["newusername"], messageData["password"], clientSocket);
        break;

    case MessageType::CHANGE_PASSWORD:
        changePassword(messageData["username"], messageData["password"], messageData["oldpassword"],
            messageData["newpassword"], messageData["confirmpassword"], clientSocket);
        break;

    case MessageType::LOGOUT:
        logout(clientSocket);
        break;

    case MessageType::DELETE_ACCOUNT:
        deleteAccount(messageData["username"], clientSocket);
        break;

    case MessageType::GET_ALL_BOOKS:
        sendAllBooks(clientSocket);
        break;
    }
}
