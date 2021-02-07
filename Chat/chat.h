#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class Chat; }
QT_END_NAMESPACE

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    Chat(QWidget *parent = nullptr);
    ~Chat();

private slots:
    void fillVector(QString nickname);
    void connect_to_server();
    void connected_to_server();
    void send();
    void recieve();
    void disconnect();
    void Quit();

private:

    int key;
    Ui::Chat *ui;
    QTcpSocket *socket;

    void listFill();
    QVector<QString> users;
    QString password_encryption(QString data);
    QString data_encryption(QString data);
    QString data_decryption(QString data);
};
#endif // CHAT_H
