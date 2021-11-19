#ifndef SERVER_HPP
#define SERVER_HPP

#include "user.hpp"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>
#include <QJsonObject>

#include <unordered_map>
#include <optional>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    virtual ~Server();

private slots:
    void newConnection();
    void sendData(QTcpSocket* clientSocket, const QJsonObject& data);
    void disconnect();

private:
    void initServer();
    void initDatabase();

    QTcpServer* tcpServer = nullptr;
    QSqlDatabase database;
    std::unordered_map<QTcpSocket*, std::optional<User>> connections;
};

#endif /* SERVER_HPP */
