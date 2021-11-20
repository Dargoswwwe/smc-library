#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtNetwork>

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

private:
    void connectToServer(const QHostAddress& address = QHostAddress::LocalHost, qint16 port = 4200);
    Ui::MainWindow* ui;
    QTcpSocket* tcpSocket;
    QDataStream inStream;
};
#endif // MAINWINDOW_HPP
