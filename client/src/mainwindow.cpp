#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });
    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });

    tcpSocket = new QTcpSocket(this);

    QObject::connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    tcpSocket->connectToHost("localhost", 4200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::connected() {
    qDebug() << "Connected to server!";
}
