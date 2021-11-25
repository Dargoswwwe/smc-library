#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "user.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpSocket(new QTcpSocket)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });
    QObject::connect(ui->buttonRegister, &QPushButton::clicked, this, [this] {
        User newUser(ui->lineRegisterUsername->text().toStdString(), ui->lineRegisterPassword->text().toStdString());
        sendData(tcpSocket,
            {

                {
                     {"messageType", "register"},
                         {
                             "data",
                           {
                             { "username", newUser.getUsername().c_str() },
                             { "password", newUser.getPassword().c_str() }
                           }
                         }
                }

            });
    });

    QObject::connect(ui->guestRegisterButton, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, [this] {
        sendData(tcpSocket, { { "hello", "test" } });
    });

    QObject::connect(ui->guestLoginButton, &QPushButton::clicked, this, [this] { switchPage(2); });


    QObject::connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    QObject::connect(tcpSocket, &QIODevice::readyRead, this, &MainWindow::receiveData);

    inStream.setDevice(tcpSocket);
    inStream.setVersion(QDataStream::Qt_5_15);

    connectToServer();
}

MainWindow::~MainWindow()
{
    tcpSocket->disconnectFromHost();
    delete ui;
}

void MainWindow::switchPage(int pageIndex) { ui->stackedWidget->setCurrentIndex(pageIndex); }

void MainWindow::connectToServer(const QHostAddress& address, qint16 port) { tcpSocket->connectToHost(address, port); }

void MainWindow::connected()
{
    qDebug() << "Connected to server!" << tcpSocket->peerAddress().toString() << ":" << tcpSocket->peerPort();
}

void MainWindow::receiveData()
{
    inStream.startTransaction();
    QString message;
    inStream >> message;
    json data=json::parse(message);

    if(data["message"]["response"]=="error")
    {
        ui->lineRegisterUsername->setText("username taken");
    }

    if(data["message"]["response"]=="success")
    {
        switchPage(2);
    }

    if (!inStream.commitTransaction()) return;
}

void MainWindow::sendData(QTcpSocket* serverSocket, const json& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << data.dump().c_str();
    serverSocket->write(block);
}
