#ifndef SERVER_HPP
#define SERVER_HPP

#include "database_manager.hpp"
#include "json.hpp"
#include "library.hpp"
#include "message_type.hpp"
#include "user.hpp"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>
#include <optional>
#include <string>
#include <unordered_map>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    virtual ~Server();

private slots:
    void newConnection();
    void sendData(QTcpSocket* clientSocket, const nlohmann::json& data);
    void receiveData();
    void disconnect();

private:
    void initServer();
    void loginUser();
    void registerUser(const std::string& name, const std::string& password, QTcpSocket* clientSocket);

    void handleMessage(QTcpSocket* clientSocket, MessageType messageType, const nlohmann::json& messageData);

    DatabaseManager database;
    QTcpServer* tcpServer = nullptr;
    std::unordered_map<QTcpSocket*, std::pair<std::optional<User>, QDataStream*>> connections;
};

#endif /* SERVER_HPP */
