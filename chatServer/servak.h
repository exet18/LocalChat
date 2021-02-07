#ifndef SERVAK_H
#define SERVAK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QVector>

#include <datebase.h>


class Servak : public QTcpServer
{
    Q_OBJECT
public:
    Servak();

private slots:
    void readyRead();
    void disconnected();
    void authorization();

protected:
    void incomingConnection(qintptr handle);
    QString password_decryption(QString data);
    QString data_decryption(QString data, QString key);


private:
    void fillVector(QString nickname);
    void sendVector();
    //void removeNick(QString nickname);
    QVector<QString> users;
    QVector<QTcpSocket*> clients;
    dateBase *ex = new dateBase("Servak","test.sqlite");
};

#endif // SERVAK_H
