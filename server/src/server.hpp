#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <memory>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);

private slots:
    void newConnection();

private:
    void initServer();
    void initDatabase();
    QTcpServer* tcpServer = nullptr;
    QSqlDatabase database;
};

#endif /* SERVER_H */
