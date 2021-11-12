#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <memory>

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);

private slots:
    void newConnection();

private:
    QTcpServer* tcpServer = nullptr;
};

#endif /* SERVER_H */
