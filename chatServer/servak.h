#ifndef SERVAK_H
#define SERVAK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

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
private:
    QVector<QTcpSocket*> clients;
    dateBase *ex = new dateBase("Servak","test.sqlite");
};

#endif // SERVAK_H
