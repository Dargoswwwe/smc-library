#include "mainwindow.hpp"

#include "bookitemwidget.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serverSocket(new QTcpSocket)
{
    // Set background
    QPixmap background(":/images_resources/library.jpg");
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

    QObject::connect(ui->buttonRegister, &QPushButton::clicked, this, &MainWindow::registerUser);
    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, &MainWindow::loginUser);

    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this, &MainWindow::showUsernameSettings);
    QObject::connect(ui->changeUsernameButton, &QPushButton::clicked, this, &MainWindow::changeUsername);
    QObject::connect(ui->changePasswordButton, &QPushButton::clicked, this, &MainWindow::changePassword);

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, &MainWindow::logoutUser);
    QObject::connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::logoutUser);

    QObject::connect(ui->deleteAccountButton, &QPushButton::clicked, this, &MainWindow::deleteAccount);

    QObject::connect(ui->buttonRegisterGuest, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->buttonLoginGuest, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this, [this] { switchPage(3); });
    QObject::connect(ui->backButton, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->logoutButton, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->deleteAccountButton, &QPushButton::clicked, this, [this] { switchPage(0); });

    QObject::connect(ui->viewMyBooksButton, &QPushButton::clicked, this, &MainWindow::getBorrowedBooks);
    QObject::connect(ui->viewAllBooksButton, &QPushButton::clicked, this, &MainWindow::getAllBooks);

    QObject::connect(serverSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    QObject::connect(serverSocket, &QIODevice::readyRead, this, &MainWindow::receiveData);

    inStream.setDevice(serverSocket);
    inStream.setVersion(QDataStream::Qt_5_15);

    connectToServer();
}

MainWindow::~MainWindow()
{
    serverSocket->disconnectFromHost();
    delete ui;
}

void MainWindow::switchPage(int pageIndex)
{
    if (pageIndex != 5) verifyConnection();

    if (pageIndex == 3 && !user.has_value()) return;
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::connectToServer(const QHostAddress& address, qint16 port)
{
    serverSocket->connectToHost(address, port);
}

void MainWindow::verifyConnection()
{
    if (serverSocket->state() == QTcpSocket::UnconnectedState) { connectToServer(); }
    int tries = 1;
    while (serverSocket->state() == QTcpSocket::UnconnectedState) {
        ui->attemptNumber->setText(std::to_string(tries).c_str());
        switchPage(5);
        QTimer::singleShot(5000, this, [this] { connectToServer(); });
        tries++;
    }
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
        qWarning() << "receiveData(): " << e.what();
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "receiveData(): " << e.what();
    }
    if (!inStream.commitTransaction()) return;

    // if (serverSocket->canReadLine()) receiveData();
}

void MainWindow::sendData(const json& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString::fromStdString(data.dump());
    serverSocket->write(block);
}

void MainWindow::registerUser()
{
    user = User(ui->lineRegisterUsername->text().toStdString(), ui->lineRegisterPassword->text().toStdString());
    json message;
    QString confirmPassword = ui->lineRegisterConfirmPassword->text();

    message["type"] = MessageType::REGISTER;
    message["data"]["username"] = user->getUsername();
    message["data"]["password"] = user->getPassword();

    User userForHashing;
    userForHashing.setPassword(confirmPassword.toStdString());
    message["data"]["confirmpassword"] = userForHashing.getPassword();

    sendData(message);
}

void MainWindow::loginUser()
{
    user = User(ui->lineLoginUsername->text().toStdString(), ui->lineLoginPassword->text().toStdString());
    json message;
    message["type"] = MessageType::LOGIN;
    message["data"]["username"] = user->getUsername();
    message["data"]["password"] = user->getPassword();

    sendData(message);
}

void MainWindow::showUsernameSettings()
{
    if (!user.has_value()) return;
    ui->changeUsernameLine->setText(user->getUsername().c_str());
}

void MainWindow::changeUsername()
{
    QString newUsername = ui->changeUsernameLine->text();
    json message;
    message["type"] = MessageType::CHANGE_USERNAME;
    message["data"]["newusername"] = newUsername.toStdString();
    message["data"]["password"] = user->getPassword();

    sendData(message);
}

void MainWindow::changePassword()
{
    QString oldPassword = ui->oldPasswordLine->text();
    QString newPassword = ui->newPasswordLine->text();
    QString confirmPassword = ui->confirmPasswordLine->text();

    User userForHashing;
    userForHashing.setPassword(oldPassword.toStdString());
    User userNewForHashing;
    userNewForHashing.setPassword(newPassword.toStdString());
    User userConfirmForHashing;
    userConfirmForHashing.setPassword(confirmPassword.toStdString());
    json message;
    message["type"] = MessageType::CHANGE_PASSWORD;
    message["data"]["username"] = user->getUsername();
    message["data"]["password"] = user->getPassword();
    message["data"]["oldpassword"] = userForHashing.getPassword();
    message["data"]["newpassword"] = userNewForHashing.getPassword();
    message["data"]["confirmpassword"] = userConfirmForHashing.getPassword();

    sendData(message);
}

void MainWindow::logoutUser()
{
    json message;
    message["type"] = MessageType::LOGOUT;

    sendData(message);
}

void MainWindow::deleteAccount()
{
    json message;
    message["type"] = MessageType::DELETE_ACCOUNT;
    message["data"]["username"] = user->getUsername();

    sendData(message);
}
void MainWindow::getBorrowedBooks()
{
    if (user.has_value() && userBooks.size() == 0) {
        json message;
        message["type"] = MessageType::GET_USER_BOOKS;
        message["data"]["username"] = user->getUsername();
        sendData(message);
        userBooks.clear();
    }
}

void MainWindow::getAllBooks()
{
    if (allBooks.size() == 0) {
        json message;
        message["type"] = MessageType::GET_ALL_BOOKS;
        sendData(message);
        allBooks.clear();
    }
}

void MainWindow::handleMessage(MessageType messageType, const json& messageData)
{
    switch (messageType) {
    case MessageType::REGISTER:
        try {
            if (messageData == "UsernameAlreadyTaken") {
                ui->labelRegisterStatus->setText("Username taken");
                user = std::nullopt;
            }
            if (messageData == "NotMatchingPasswords") {
                ui->labelRegisterStatus->setText("Passwords don't match!");
                user = std::nullopt;
            }
            if (messageData == "PasswordAlreadyTaken") {
                ui->labelRegisterStatus->setText("Password already taken. Type another password!");
                user = std::nullopt;
            }
            if (messageData == "Success") {
                switchPage(2);
                // requestAllBooks();
            }
            if (messageData == "EmptyNameField") {
                ui->labelRegisterStatus->setText("The username filed is empty!");
                user = std::nullopt;
            }
            if (messageData == "EmptyPasswordField") {
                ui->labelRegisterStatus->setText("The password filed is empty!");
                user = std::nullopt;
            }
            if (messageData == "EmptyFields") {
                ui->labelRegisterStatus->setText("The fileds are empty!");
                user = std::nullopt;
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

    case MessageType::CHANGE_USERNAME:
        try {
            if (messageData == "AlreadyTaken")
                ui->changeUsernamePasswordLabel->setText("This username is already taken.");
            if (messageData == "Success") {
                user->setUsername(ui->changeUsernameLine->text().toStdString());
                ui->changeUsernamePasswordLabel->setText("Username updated!");
            }
        } catch (const nlohmann::detail::type_error& e) { }
        break;

    case MessageType::CHANGE_PASSWORD:

        try {
            if (messageData == "IncorrectOldPassword")
                ui->changeUsernamePasswordLabel->setText("The old password is incorrect!");
            if (messageData == "SameWithOldPassword")
                ui->changeUsernamePasswordLabel->setText("Type a different password from the old one!");
            if (messageData == "NotMatchingPasswords")
                ui->changeUsernamePasswordLabel->setText("Passwords don't match!");
            if (messageData == "Success") {
                user->setPassword(ui->newPasswordLine->text().toStdString());
                ui->changeUsernamePasswordLabel->setText("Password updated!");
            }

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
        } catch (const nlohmann::detail::type_error& e) {
            qWarning() << "LOGOUT: " << e.what();
        }
        break;

    case MessageType::DELETE_ACCOUNT:
        try {
            if (messageData == "Success") {
                switchPage(0);
                ui->lineLoginUsername->setText("");
                ui->lineLoginPassword->setText("");
                user = std::nullopt;
            }
        } catch (const nlohmann::detail::type_error& e) {
            qWarning() << "DELETE_ACCOUNT: " << e.what();
        }
        break;

    case MessageType::GET_ALL_BOOKS:
        try {
            for (Book b : messageData) {
                BookItemWidget* bookItem = new BookItemWidget(b, ui->scrollAreaWidgetContents);
                allBooks.push_back(b);
                ui->verticalLayout_4->addWidget(bookItem);
            }
        } catch (const nlohmann::detail::type_error& e) {
            qWarning() << "GET_ALL_BOOKS: " << e.what();
        }
        break;

    case MessageType::GET_USER_BOOKS:
        try {
            for (Book b : messageData) {
                BookItemWidget* bookItem = new BookItemWidget(b, ui->scrollAreaWidgetContents);
                userBooks.push_back(b);
                ui->verticalLayout_4->addWidget(bookItem);
            }
        } catch (const nlohmann::detail::type_error& e) {
            qWarning() << "GET_USER_BOOKS: " << e.what();
        }
        break;

    case MessageType::FINISHED:
        try {
            ui->verticalLayout_4->addStretch();
        } catch (const nlohmann::detail::type_error& e) {
            qWarning() << "FINISHED: " << e.what();
        }
        break;
    }
}
