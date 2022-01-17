#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "book.hpp"
#include "json.hpp"
#include "message_type.hpp"
#include "user.hpp"

#include <QMainWindow>
#include <QtNetwork>
#include <QtWidgets>
#include <optional>

using nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::optional<User> user;
    std::vector<Book> allBooks;
    std::vector<Book> userBooks;
    std::unordered_map<MessageType, std::function<void(json const&)>> messageHandler;
    std::unordered_map<std::string, QDate> borrowedDates;
    QTimer refreshTimer;

    bool passwordChecks(std::string const& password, std::string const& confirmPassword, QLabel* errorLabel);

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void switchPage(int pageIndex);
    void connected();
    void receiveData();
    void sendData(const json& data);

    void registerUser();
    void loginUser();
    void logoutUser();
    void showUsernameSettings();
    void changeUsername();
    void changePassword();
    void deleteAccount();
    void getBorrowedBooks();
    void getAllBooks();
    void borrowBook(std::string book_title);
    void returnBook(std::string book_title);
    void popupMessage(std::string message);
    void getBorrowedDate(std::string book_title);

signals:
    void receivedBorrowDate(std::string bookTitle, QDate borrowedDate);

private:
    void connectToServer(const QHostAddress& address = QHostAddress::LocalHost, qint16 port = 4200);
    void verifyConnection();
    void clearBooksUi();

    void handleRegister(const json& messageData);
    void handleLogin(const json& messageData);
    void handleChangeUsername(const json& messageData);
    void handleChangePassword(const json& messageData);
    void handleLogout(const json& messageData);
    void handleDelete(const json& messageData);
    void handleGetAllBooks(const json& messageData);
    void handleGetUserBooks(const json& messageData);
    void handleFinished();
    void handleBorrowBook(const json& messageData);
    void handleReturnBook(const json& messageData);
    void handleGetDate(const json& messageData);

    Ui::MainWindow* ui;
    QTcpSocket* serverSocket;
    QDataStream inStream;
};
#endif // MAINWINDOW_HPP
