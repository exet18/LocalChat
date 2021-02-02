#include "servak.h"

Servak::Servak()
{

}

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

void Servak::incomingConnection(qintptr handle)
{
    QTcpSocket *client = new QTcpSocket();
    client->setSocketDescriptor(handle);
    clients.append(client);

    qDebug() << "New connection from: " << client->peerAddress().toString();

    connect (client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    connect (client, &QTcpSocket::disconnected, this, &Servak::disconnected);
}

QString Servak::passwordDecryption(QString data)
{
    for(int iter = 0; iter < data.length(); iter++){
            char sym = QString(data).at(iter).toLatin1();
            sym -= 4;
            data.replace(iter,1,sym);
        }
        return data;
}

QString Servak::dataDecryption(QString data)
{
    for(int iter = 0; iter < data.length(); iter++){
            char sym = QString(data).at(iter).toLatin1();
            if(sym >= 'a' && sym <= 'z'){
                if(sym-8 > 'z'){
                    sym += 18;
                }
                else{
                    sym -= 8;
                }
            }
            else if(sym >= 'A' && sym <= 'Z'){
                if(sym-8 > 'Z'){
                    sym += 18;
                }
                else{
                    sym -= 8;
                }
            }
            data.replace(iter,1,sym);
        }
        return data;
}
