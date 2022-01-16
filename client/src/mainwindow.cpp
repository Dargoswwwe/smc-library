#include "mainwindow.hpp"

#include "bookitemwidget.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serverSocket(new QTcpSocket)
{
    refreshTimer.setInterval(1000);
    refreshTimer.start();
    connect(&refreshTimer, &QTimer::timeout, this, &MainWindow::receiveData);

    messageHandler = {
        { MessageType::REGISTER, [this] (const json& messageData) { handleRegister(messageData); } },
        { MessageType::LOGIN, [this] (const json& messageData) { handleLogin(messageData); } },
        { MessageType::CHANGE_USERNAME, [this] (const json& messageData) { handleChangeUsername(messageData); } },
        { MessageType::CHANGE_PASSWORD, [this] (const json& messageData) { handleChangePassword(messageData); } },
        { MessageType::LOGOUT, [this] (const json& messageData) { handleLogout(messageData); } },
        { MessageType::DELETE_ACCOUNT, [this] (const json& messageData) { handleDelete(messageData); } },
        { MessageType::GET_ALL_BOOKS, [this] (const json& messageData) { handleGetAllBooks(messageData); } },
        { MessageType::GET_USER_BOOKS, [this] (const json& messageData) { handleGetUserBooks(messageData); } },
        { MessageType::GET_BORROWED_DATE, [this] (const json& messageData) { handleGetDate(messageData); } },
        { MessageType::FINISHED, [this] (const json& messageData) { handleFinished(); } },
        { MessageType::BORROW_BOOK, [this] (const json& messageData) { handleBorrowBook(messageData); } },
        { MessageType::RETURN_BOOK, [this] (const json& messageData) { handleReturnBook(messageData); } },
        { MessageType::RETURN_BOOK, [this] (const json& messageData) { handleGetDate(messageData); } }
    };

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
    QObject::connect(ui->lineRegisterConfirmPassword, &QLineEdit::returnPressed, ui->buttonRegister, &QPushButton::click);

    QObject::connect(ui->buttonRegister, &QPushButton::clicked, this, &MainWindow::registerUser);
    QObject::connect(ui->buttonLogin, &QPushButton::clicked, this, &MainWindow::loginUser);

    QObject::connect(ui->buttonLoginInstead, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->buttonRegisterInstead, &QPushButton::clicked, this, [this] { switchPage(1); });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this, &MainWindow::showUsernameSettings);
    QObject::connect(ui->changeUsernameButton, &QPushButton::clicked, this, &MainWindow::changeUsername);
    QObject::connect(ui->changePasswordButton, &QPushButton::clicked, this, &MainWindow::changePassword);

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, &MainWindow::logoutUser);
    QObject::connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::logoutUser);

    QObject::connect(ui->buttonRegisterGuest, &QPushButton::clicked, this, [this] { switchPage(2); });
    QObject::connect(ui->buttonLoginGuest, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->accountSettingsButton, &QPushButton::clicked, this, [this] { switchPage(3); });
    QObject::connect(ui->backButton, &QPushButton::clicked, this, [this] { switchPage(2); });

    QObject::connect(ui->logoutButton_2, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->logoutButton, &QPushButton::clicked, this, [this] { switchPage(0); });
    QObject::connect(ui->deleteAccountButton, &QPushButton::clicked, this, &MainWindow::deleteAccount);
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
    if (pageIndex != 5)
        verifyConnection();

    if (pageIndex == 3 && !user.has_value())
        return;
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::connectToServer(const QHostAddress& address, qint16 port)
{
    serverSocket->connectToHost(address, port);
}

void MainWindow::verifyConnection()
{
    if (serverSocket->state() == QTcpSocket::UnconnectedState) {
        connectToServer();
    }
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
    if (inStream.atEnd()) return;
    inStream.startTransaction();
    QString messageString;
    inStream >> messageString;

    json message = json::parse(messageString.toStdString());

    qDebug() << message.dump().c_str();

    try {
        MessageType messageType = message["type"];
        messageHandler.at(messageType)(message["data"]);
    } catch (const nlohmann::detail::parse_error& e) {
        qWarning() << "receiveData(): " << e.what();
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "receiveData(): " << e.what();
    }
    if (!inStream.commitTransaction())
        return;

    if (serverSocket->canReadLine())
        receiveData();
}

void MainWindow::sendData(const json& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << QString::fromStdString(data.dump());
    serverSocket->write(block);
}

bool MainWindow::passwordChecks(std::string const& password, std::string const& confirmPassword, QLabel* errorLabel)
{
    if (password != confirmPassword) {
        errorLabel->setText("Passwords don't match!");
        return false;
    }

    if (password.length() < 8) {
        errorLabel->setText("The password must have at least 8 characters");
        return false;
    }

    return true;
}

void MainWindow::registerUser()
{
    auto username = ui->lineRegisterUsername->text().toStdString();
    auto password = ui->lineRegisterPassword->text().toStdString();
    auto confirmPassword = ui->lineRegisterConfirmPassword->text().toStdString();

    if (username.length() < 4) {
        ui->labelRegisterStatus->setText("Username too short");
        return;
    }

    if (!passwordChecks(password, confirmPassword, ui->labelRegisterStatus))
        return;

    json message;
    user = User(username, password, username);

    message["type"] = MessageType::REGISTER;
    message["data"]["user"] = *user;

    ui->lineRegisterUsername->setText("");
    ui->lineRegisterPassword->setText("");
    ui->lineRegisterConfirmPassword->setText("");

    sendData(message);
}

void MainWindow::loginUser()
{
    std::string username = ui->lineLoginUsername->text().toStdString();
    std::string password = ui->lineLoginPassword->text().toStdString();

    user = User(username, password, username);
    json message;
    message["type"] = MessageType::LOGIN;
    message["data"]["user"] = *user;

    sendData(message);
}

void MainWindow::showUsernameSettings()
{
    if (!user.has_value())
        return;
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
    if (user.has_value()) {
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

void MainWindow::borrowBook(std::string book_title)
{
    json message;
    message["type"] = MessageType::BORROW_BOOK;
    message["data"]["booktitle"] = book_title;
    message["data"]["username"] = user->getUsername();

    sendData(message);
}

void MainWindow::returnBook(std::string book_title)
{
    json message;
    message["type"] = MessageType::RETURN_BOOK;
    message["data"]["booktitle"] = book_title;
    message["data"]["username"] = user->getUsername();

    sendData(message);
}

void MainWindow::getBorrowedDate(std::string book_title)
{
    json message;
    message["type"] = MessageType::GET_BORROWED_DATE;
    message["data"]["booktitle"] = book_title;
    message["data"]["username"] = user->getUsername();

    sendData(message);
}

void MainWindow::popupMessage(std::string message)
{
    QMessageBox::information(
        this,
        "Info",
        message.c_str());
}

void MainWindow::handleRegister(const json& messageData)
{
    try {
        if (messageData == "UsernameAlreadyTaken") {
            ui->labelRegisterStatus->setText("Username taken");
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
    } catch (const nlohmann::detail::type_error& e) {
    }
}

void MainWindow::handleLogin(const json& messageData)
{
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
    } catch (const nlohmann::detail::type_error& e) {
    }
}

void MainWindow::handleChangeUsername(const json& messageData)
{
    try {
        if (messageData == "AlreadyTaken")
            ui->changeUsernamePasswordLabel->setText("This username is already taken.");
        if (messageData == "Success") {
            user->setUsername(ui->changeUsernameLine->text().toStdString());
            ui->changeUsernamePasswordLabel->setText("Username updated!");
        }
    } catch (const nlohmann::detail::type_error& e) {
    }
}

void MainWindow::handleChangePassword(const json& messageData)
{
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

    } catch (const nlohmann::detail::type_error& e) {
    }
}

void MainWindow::handleLogout(const json& messageData)
{
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
}

void MainWindow::handleDelete(const json& messageData)
{
    try {
        if (messageData == "Success") {
            std::string info = "Account deleted";
            popupMessage(info);
            switchPage(0);
            ui->lineLoginUsername->setText("");
            ui->lineLoginPassword->setText("");
            user = std::nullopt;
        }
        if (messageData == "BorrowedBooks") {
            std::string info = "You have to return the borrowed books first!";
            popupMessage(info);
            switchPage(2);
        }
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "DELETE_ACCOUNT: " << e.what();
    }
}

void MainWindow::handleGetAllBooks(const json& messageData)
{
    try {
        for (Book b : messageData) {
            BookItemWidget* bookItem = new BookItemWidget(b, ui->scrollAreaWidgetContents);
            allBooks.push_back(b);
            ui->verticalLayout_4->addWidget(bookItem);
        }
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "GET_ALL_BOOKS: " << e.what();
    }
}

void MainWindow::handleGetUserBooks(const json& messageData)
{
    try {
        for (Book b : messageData) {
            BookItemWidget* bookItem = new BookItemWidget(b, ui->scrollAreaWidgetContents);
            userBooks.push_back(b);
            ui->verticalLayout_4->addWidget(bookItem);
        }
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "GET_USER_BOOKS: " << e.what();
    }
}

void MainWindow::handleFinished()
{
    try {
        ui->verticalLayout_4->addStretch();
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "FINISHED: " << e.what();
    }
}

void MainWindow::handleBorrowBook(const json& messageData)
{
    try {
        if (messageData == "NotAvailable") {
            std::string info = "This book is not available at the moment! Please check it later!";
            popupMessage(info);
        }
        if (messageData == "Success") {
            std::string info = "Book borrowed succesfully, enjoy your reading!";
            popupMessage(info);
        }
        if (messageData == "AlreadyFiveBooks") {
            std::string info = "You have already five borrowed books!";
            popupMessage(info);
        }
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "BORROW_BOOK: " << e.what();
    }
}

void MainWindow::handleReturnBook(const json& messageData)
{
    try {
        if (messageData == "Success") {
            std::string info = "Book returned succesfully!";
            popupMessage(info);
        }
    } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "RETURN_BOOK: " << e.what();
    }
}

void MainWindow::handleGetDate(const json& messageData)
{
    try {
        QString dateString = messageData["date"].get<std::string>().c_str();
        if (!dateString.length()) return;
        QDate borrowedDate = QDate::fromString(dateString, Qt::ISODate);
        std::string bookTitle = messageData["booktitle"];
        borrowedDates[bookTitle] = QDate::fromString(dateString, Qt::ISODate);
        emit receivedBorrowDate(bookTitle, borrowedDate);
     } catch (const nlohmann::detail::type_error& e) {
        qWarning() << "GET_DATE: " << e.what();
    }
}
