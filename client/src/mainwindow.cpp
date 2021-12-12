#include "mainwindow.hpp"

#include "bookitemwidget.hpp"
#include "ui_mainwindow.h"

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
        user = User(ui->lineRegisterUsername->text().toStdString(), ui->lineRegisterPassword->text().toStdString());
        json message;
        message["type"] = MessageType::REGISTER;
        message["data"]["username"] = user->getUsername();
        message["data"]["password"] = user->getPassword();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, [this] {
        user = User(ui->lineLoginUsername->text().toStdString(), ui->lineLoginPassword->text().toStdString());
        json message;
        message["type"] = MessageType::LOGIN;
        message["data"]["username"] = user->getUsername();
        message["data"]["password"] = user->getPassword();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this,
        [this] { ui->changeUsernameLine->setText(user->getUsername().c_str()); });

    QObject::connect(ui->changeUsernameButton, &QPushButton::clicked, this, [this] {
        QString newUsername = ui->changeUsernameLine->text();
        json message;
        message["type"] = MessageType::CHANGE_USERNAME;
        message["data"]["newusername"] = newUsername.toStdString();
        message["data"]["password"] = user->getPassword();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->changePasswordButton, &QPushButton::clicked, this, [this] {
        QString oldPassword = ui->oldPasswordLine->text();
        QString newPassword = ui->newPasswordLine->text();
        QString confirmPassword = ui->confirmPasswordLine->text();

        User userForHashing;
        userForHashing.setPassword(oldPassword.toStdString());
        json message;
        message["type"] = MessageType::CHANGE_PASSWORD;
        message["data"]["username"] = user->getUsername();
        message["data"]["password"] = user->getPassword();
        message["data"]["oldpassword"] = userForHashing.getPassword();
        message["data"]["newpassword"] = newPassword.toStdString();
        message["data"]["confirmpassword"] = confirmPassword.toStdString();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, [this] {
        json message;
        message["type"] = MessageType::LOGOUT;

        sendData(serverSocket, message);
    });

    QObject::connect(ui->logoutButton_4, &QPushButton::clicked, this, [this] {
        json message;
        message["type"] = MessageType::LOGOUT;

        sendData(serverSocket, message);
    });

    QObject::connect(ui->deleteAccountButton, &QPushButton::clicked, this, [this] {
        json message;
        message["type"] = MessageType::DELETE_ACCOUNT;
        message["data"]["username"] = user->getUsername();

        sendData(serverSocket, message);
    });

    QObject::connect(ui->buttonRegisterGuest, &QPushButton::clicked, this, [this] { switchPage(2); });
    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });

    QObject::connect(ui->buttonLoginGuest, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this, [this] { switchPage(3); });
    QObject::connect(ui->backButton, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->logoutButton_4, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->deleteAccountButton, &QPushButton::clicked, this, [this] { switchPage(0); });

    QObject::connect(serverSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    QObject::connect(serverSocket, &QIODevice::readyRead, this, &MainWindow::receiveData);

    inStream.setDevice(serverSocket);
    inStream.setVersion(QDataStream::Qt_5_15);

    //    BookItemWidget* bookItem = new BookItemWidget(book,ui->scrollAreaWidgetContents);
    //    ui->verticalLayout_4->addWidget(bookItem);

    //    Book book;

    //    for (int i = 0; i < 10; i++) {
    //            BookItemWidget* bookItem = new BookItemWidget(book, ui->scrollAreaWidgetContents);
    //            ui->verticalLayout_4->addWidget(bookItem);
    //        }

    //    ui->verticalLayout_4->addStretch();

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
            if (messageData == "Error") {
                ui->labelRegisterStatus->setText("Username taken");
                user = std::nullopt;
            }

            if (messageData == "Success") {
                switchPage(2);
                // requestAllBooks();
            }
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::LOGIN:
        try {
            if (messageData == "NameError") {
                ui->labelLoginStatus->setText("This username is not registered.");
                user = std::nullopt;
            }
            if (messageData == "PasswordError") {
                ui->labelLoginStatus->setText("Wrong password.");
                user = std::nullopt;
            }
            if (messageData == "Success") {
                switchPage(2);
                // requestAllBooks();
            }
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::GET_BOOKS:
        break;

    case MessageType::CHANGE_USERNAME:
        try {
            if (messageData == "AlreadyTaken") ui->changeUsernameLine->setText("This username is already taken.");
            if (messageData == "Success") user->setUsername(ui->changeUsernameLine->text().toStdString());
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::CHANGE_PASSWORD:

        try {
            if (messageData == "IncorrectOldPassword")
                ui->changePasswordErrorLabel->setText("The old password is incorrect!");
            if (messageData == "SameWithOldPassword")
                ui->changePasswordErrorLabel->setText("Type a different password from the old one!");
            if (messageData == "NotMatchingPasswords") ui->changePasswordErrorLabel->setText("Passwords don't match!");
            if (messageData == "Success") user->setPassword(ui->newPasswordLine->text().toStdString());

        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::LOGOUT:
        try {
            if (messageData == "Success") {
                switchPage(0);
                ui->lineLoginUsername->setText("");
                ui->lineLoginPassword->setText("");
                user = std::nullopt;
            }
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::DELETE_ACCOUNT:
        try {
            if (messageData == "Success") {
                switchPage(0);
                ui->lineLoginUsername->setText("");
                ui->lineLoginPassword->setText("");
                user = std::nullopt;
            }
        } catch (const nlohmann::detail::type_error& e) { }
        break;
    }
}
