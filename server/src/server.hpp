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
    void receiveData();
    void disconnect();

private:
    void initServer();

    QTcpServer* tcpServer = nullptr;
    QSqlDatabase database;
    std::unordered_map<QTcpSocket*, std::pair<std::optional<User>, QDataStream*>> connections;
};

#endif /* SERVER_HPP */
