#include "mainwindow.hpp"

#include "bookitemwidget.hpp"
#include "ui_mainwindow.h"
#include "user.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serverSocket(new QTcpSocket)
{
    // Set background
    QPixmap background(":/images_resources/images/library.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    ui->setupUi(this);

    QObject::connect(ui->lineLoginUsername, &QLineEdit::returnPressed, ui->buttonLogin, &QPushButton::click);
    QObject::connect(ui->lineLoginPassword, &QLineEdit::returnPressed, ui->buttonLogin, &QPushButton::click);
    QObject::connect(ui->lineRegisterUsername, &QLineEdit::returnPressed, ui->buttonRegister, &QPushButton::click);
    QObject::connect(ui->lineRegisterPassword, &QLineEdit::returnPressed, ui->buttonRegister, &QPushButton::click);
    QObject::connect(
                ui->lineRegisterConfirmPassword, &QLineEdit::returnPressed, ui->buttonRegister, &QPushButton::click);
    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });

    QObject::connect(ui->buttonRegister, &QPushButton::clicked, this, [this] {
        User newUser(ui->lineRegisterUsername->text().toStdString(), ui->lineRegisterPassword->text().toStdString());
        json message;
        message["type"] = MessageType::REGISTER;
        message["data"]["username"] = newUser.getUsername();
        message["data"]["password"] = newUser.getPassword();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, [this] {
        User newUser(ui->lineRegisterUsername->text().toStdString(), ui->lineRegisterPassword->text().toStdString());
        json message;
        message["type"] = MessageType::LOGIN;
        message["data"]["username"] = newUser.getUsername();
        message["data"]["password"] = newUser.getPassword();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->buttonRegisterGuest, &QPushButton::clicked, this, [this] { switchPage(2); });
    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(
                ui->buttonLogin, &QPushButton::clicked, this, [this] { sendData(serverSocket, R"({"hello": "test"})"_json); });

    QObject::connect(ui->buttonLoginGuest, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, [this] { switchPage(3); });

    QObject::connect(serverSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    QObject::connect(serverSocket, &QIODevice::readyRead, this, &MainWindow::receiveData);

    inStream.setDevice(serverSocket);
    inStream.setVersion(QDataStream::Qt_5_15);

//    BookItemWidget* bookItem = new BookItemWidget(book,ui->scrollAreaWidgetContents);
//    ui->scrollVerticalLayout->addWidget(bookItem);

    ui->scrollVerticalLayout->addStretch();

    connectToServer();
}

MainWindow::~MainWindow()
{
    serverSocket->disconnectFromHost();
    delete ui;
}

void MainWindow::switchPage(int pageIndex) { ui->stackedWidget->setCurrentIndex(pageIndex); }

void MainWindow::connectToServer(const QHostAddress& address, qint16 port)
{
    serverSocket->connectToHost(address, port);
}

void MainWindow::connected()
{
    qDebug() << "Connected to server!" << serverSocket->peerAddress().toString() << ":" << serverSocket->peerPort();
}

void MainWindow::receiveData()
{
    inStream.startTransaction();
    QString messageString;
    inStream >> messageString;

    json message = json::parse(messageString.toStdString());

    qDebug() << message.dump().c_str();

    try {

        handleMessage(message["type"], message["data"]);
    } catch (const nlohmann::detail::parse_error& e) {
        qWarning() << e.what();
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << e.what();
    }
    if (!inStream.commitTransaction()) return;
}

void MainWindow::sendData(QTcpSocket* serverSocket, const json& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString::fromStdString(data.dump());
    serverSocket->write(block);
}

void MainWindow::handleMessage(MessageType messageType, const json& messageData)
{
    switch (messageType) {
    case MessageType::REGISTER:
        try {
        if (messageData["response"] == "error") { ui->lineRegisterUsername->setText("username taken"); }

        if (messageData["response"] == "success") { switchPage(2); }
    } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::LOGIN:
        try {
        if (messageData["response"] == "name error") { ui->lineLoginUsername->setText("username not registered"); }
        if (messageData["response"] == "password error") { ui->lineLoginUsername->setText("wrong password"); }
        if (messageData["response"] == "success") { switchPage(2); }
    } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::GET_BOOKS:
        break;
    }
}
