#include "servak.h"
#include "datebase.h"

Servak::Servak()
{

}

void Servak::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray byteData = client->readAll();
    QString data(byteData);

    /*if(data[0] == 'r')
    {
        QString s = "";

        data.remove(0,1);
        for(QTcpSocket *buffer : clients)
            buffer->write(byteData);

        for(int i=0; data[i] != ' '; i++)
            s += data[i];
        removeNick(s);
        sendVector();
    }*/
    if(data[0] == '|')
    {
        fillVector(data+' ');
        sendVector();
    }
    else
        for(QTcpSocket *buffer : clients)
            buffer->write(byteData);
}

/*void Servak::removeNick(QString nickname)
{
    for(int i=0; i<users.length(); i++)
        if(users[i] == nickname)
            users.remove(i);
}*/

void Servak::fillVector(QString nickname)
{
    bool found = false;

    for(int i=0; i<users.length(); i++)
        if(users[i] == nickname)
            found = true;
    if(!found)
        users.append(nickname);
}

void Servak::sendVector()
{
    for(int i=0; i<users.length(); i++)
        for(QTcpSocket *buffer : clients)
            buffer->write((users[i]).toUtf8());
}

void Servak::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client " << client->peerAddress().toString() << " disconnected!";

    disconnect(client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    disconnect(client, &QTcpSocket::disconnected, this, &Servak::disconnected);

    clients.remove(clients.indexOf(client));
}

void Servak::authorization()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QString login = QString::fromUtf8(client->readLine());
    QString pass = QString::fromUtf8(client->readLine());
    QString key = QString::fromUtf8(client->readLine());

    pass = password_decryption(pass);
    login.chop(1);
    pass.chop(1);
    login = data_decryption(login,key);

    if(ex->open_connection())
    {
        if(!ex->add_User(login,pass))
        {
            if(ex->check_password(login, pass)!=0)
                client->abort();
            /*else
            {
                QByteArray data = client->readLine();
                QString nickname(data);

                if(nickname[0] == '|')
                {
                    fillVector(nickname+' ');
                    sendVector();
                }
            }*/
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

    connect(client, &QTcpSocket::readyRead, this, &Servak::authorization);
    connect(client, &QTcpSocket::readyRead, this, &Servak::readyRead);
    connect(client, &QTcpSocket::disconnected, this, &Servak::disconnected);
}

QString Servak::password_decryption(QString data)
{
    for(int iter = 0; iter < data.length(); iter++)
    {
        char sym = QString(data).at(iter).toLatin1();
        sym -= 4;
        data.replace(iter,1,sym);
    }
    return data;
}

QString Servak::data_decryption(QString data,QString key)
{

    for(int iter = 0; iter < data.length(); iter++)
    {
        char ch = data.at(iter).toLatin1();

        if(ch >= 'a' && ch <= 'z')
        {
            ch = ch - key.toInt();
            if(ch < 'a')
                ch = ch + 'z' - 'a' + 1;

            data[iter] = ch;
        }
        else if(ch >= 'A' && ch <= 'Z')
        {
            ch = ch - key.toInt();
            if(ch > 'a')
                ch = ch + 'Z' - 'A' + 1;

            data[iter] = ch;
        }
    }
    return data;
}
