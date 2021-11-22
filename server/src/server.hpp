#ifndef SERVER_HPP
#define SERVER_HPP

#include "database_manager.hpp"
#include "user.hpp"

#include <QJsonObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>

#include <optional>
#include <unordered_map>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    virtual ~Server();

private slots:
    void newConnection();
    void sendData(QTcpSocket* clientSocket, const QJsonObject& data);
    void receiveData();
    void disconnect();

private:
    void initServer();

    DatabaseManager database;
    QTcpServer* tcpServer = nullptr;
    std::unordered_map<QTcpSocket*, std::pair<std::optional<User>, QDataStream*>> connections;
};

#endif /* SERVER_HPP */
