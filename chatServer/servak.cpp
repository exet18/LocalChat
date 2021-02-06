#include "servak.h"
#include "datebase.h"

Servak::Servak(){};

void Servak::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray data = client->readAll();
    for (QTcpSocket *buffer : clients)
    {
        buffer->write(data);
    }
}

void Servak::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client " << client->peerAddress().toString() << " disconnected!";

    disconnect (client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    disconnect (client, &QTcpSocket::disconnected, this, &Servak::disconnected);

    clients.remove(clients.indexOf(client));
}

void Servak::authorization()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QString login = QString::fromUtf8(client->readLine());
    QString pass = QString::fromUtf8(client->readLine());
    login.chop(1);
    pass.chop(1);
    if(ex->open_connection())
    if(!ex->add_User(login,pass))
    {
        if(ex->check_password(login, pass)!=0)
           {
               client->abort();
           }
       }
    ex->close_connection();
    disconnect(client, &QTcpSocket::readyRead, this, &Servak::authorization);
}

void Servak::incomingConnection(qintptr handle)
{
    QTcpSocket *client = new QTcpSocket();
    client->setSocketDescriptor(handle);
    clients.append(client);

    qDebug() << "New connection from: " << client->peerAddress().toString();
    connect (client, &QTcpSocket::readyRead, this, &Servak::authorization);
    connect (client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    connect (client, &QTcpSocket::disconnected, this, &Servak::disconnected);
}
