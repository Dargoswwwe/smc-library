#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#define JSON_STD_FILESYSTEM

#include "json.hpp"

#include <QMainWindow>
#include <QtNetwork>

using nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void switchPage(int pageIndex);
    void connected();
    void receiveData();
    void sendData(QTcpSocket* serverSocket, const json& data);

private:
    void connectToServer(const QHostAddress& address = QHostAddress::LocalHost, qint16 port = 4200);
    Ui::MainWindow* ui;
    QTcpSocket* serverSocket;
    QDataStream inStream;
};
#endif // MAINWINDOW_HPP
