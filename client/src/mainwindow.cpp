#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpSocket(new QTcpSocket)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });
    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });

    QObject::connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::connected);

    connectToServer();
}

MainWindow::~MainWindow()
{
    tcpSocket->disconnectFromHost();
    delete ui;
}

void MainWindow::switchPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::connectToServer(const QHostAddress& address, qint16 port) {
    tcpSocket->connectToHost(address, port);
}

void MainWindow::connected() {
    qDebug() << "Connected to server!" << tcpSocket->peerAddress().toString() << ":" << tcpSocket->peerPort();
}
