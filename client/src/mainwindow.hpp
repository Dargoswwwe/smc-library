#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "book.hpp"
#include "json.hpp"
#include "message_type.hpp"
#include "user.hpp"

#include <QMainWindow>
#include <QtNetwork>
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

private:
    void connectToServer(const QHostAddress& address = QHostAddress::LocalHost, qint16 port = 4200);
    void handleMessage(MessageType messageType, const nlohmann::json& messageData);
    void verifyConnection();
    Ui::MainWindow* ui;
    QTcpSocket* serverSocket;
    QDataStream inStream;
};
#endif // MAINWINDOW_HPP
