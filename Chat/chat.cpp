#include "chat.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Chat)
{
    ui->setupUi(this);

    ui->text_messages->setReadOnly(true);

    socket = new QTcpSocket();
    ui->sb_port->setRange(0,65535);

    ui->e_address->setText("127.0.0.1");
    ui->sb_port->setValue(8080);

    connect(ui->b_connect, &QPushButton::clicked, this, &Chat::connect_to_server);
    connect(ui->b_send, &QPushButton::clicked, this, &Chat::send);
    connect(ui->b_disconnect, &QPushButton::clicked, this, &Chat::disconnect);
    connect(ui->Quit, &QPushButton::clicked, this, &Chat::Quit);

    connect(socket, &QTcpSocket::readyRead, this, &Chat::recieve);
    connect(socket, &QTcpSocket::connected, this, &Chat::connected_to_server);
    connect(socket,&QTcpSocket::disconnected, this, &Chat::disconnect);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::connect_to_server()
{
    QString ip = ui->e_address->text();
    QString nickname = ui->e_nickname->text();
    key = ui->key_input->text().toInt();
    if(key > 0 and key < 27)
    {
        QString password = ui->password->text();
        int port = ui->sb_port->value();
        socket->connectToHost(ip, port);
        password = password_encryption(password);

        socket->write(data_encryption(nickname +'\n').toUtf8());
        socket->write((password + '\n').toUtf8());
        socket->write((QString (key)+'\n').toUtf8());
    }
}

void Chat::connected_to_server()
{
    ui->stackedWidget->setCurrentWidget(ui->messages);
    ui->statusbar->showMessage("Connected to " " "+ ui->e_address->text());

    if(ui->stackedWidget->currentWidget() == ui->messages)
        socket->write(data_encryption('|'+ui->e_nickname->text()).toUtf8());
}

void Chat::send()
{
    QString message = data_encryption(ui->e_nickname->text()) + ": " + data_encryption(ui->send_message->toPlainText());
    socket->write(message.toUtf8());
    ui->send_message->clear();
}

void Chat::listFill()
{
    ui->listWidget->clear();
    for(int i=0; i<users.length(); i++)
        ui->listWidget->addItem(users[i]);
    ui->text_messages->setHtml(ui->text_messages->toHtml() + users.last() + " has connected to the server.");
}

void Chat::fillVector(QString nickname)
{
    QString tmp = "";
    users.clear();

    for(int i=0; nickname[i]!='\0';i++)
    {
        if(nickname[0] == '|')
            nickname.remove(0,1);
        if(nickname[i] == ' ')
        {
            for(int j=0; j<i; j++)
                tmp += nickname[j];

            nickname.remove(0,i+1);
            users.append(tmp);
            tmp = "";
        }
    }
}

void Chat::recieve()
{
    QString message = data_decryption(QString::fromUtf8(socket->readAll()).toLatin1());

    if(message[0] == '|')
    {
        fillVector(message);
        listFill();
    }
    else
        ui->text_messages->setHtml(ui->text_messages->toHtml() + message);
}

void Chat::disconnect()
{
    socket->write('r'+data_encryption(ui->e_nickname->text() + " has disconnect from server.").toUtf8());
    socket->disconnectFromHost();
    ui->statusbar->showMessage("Disconnected from" " " + ui->e_address->text());
    ui->stackedWidget->setCurrentWidget(ui->connection_page);
}

void Chat::Quit()
{
    close();
}

QString Chat::password_encryption(QString data)
{
    for(int iter = 0; iter < data.length(); iter++)
    {
        char sym = QString(data).at(iter).toLatin1();
        sym += 4;
        data.replace(iter,1,sym);
    }
    return data;
}

QString Chat::data_encryption(QString data)
{
    int rest;
    for(int iter = 0; iter < data.length(); iter++)
    {
        char ch = data.at(iter).toLatin1();
        if(ch >= 97 && ch <= 122)
        {
            ch += key;
            if(ch > 122 || ch < -1)
            {
                rest = ch-122;
                ch = 96 + rest;
            }
        }
        else if(ch >= 65 && ch <= 90)
        {
            ch = ch + key;
            if(ch > 90)
            {
                rest = ch-90;
                ch = 64 + rest;
            }
        }
        data[iter] = ch;
    }    
    return data;
}

QString Chat::data_decryption(QString data)
{
    int rest;
    for(int iter = 0; iter < data.length(); iter++)
    {
        char ch = data.at(iter).toLatin1();
        if(ch >= 97 && ch <= 122)
        {
            ch -= key;
            if(ch < 97)
        {
            rest = 26 - key;
            ch += rest + key;
        }
    }
    else if(ch >= 65 && ch <= 90)
    {
        ch -= key;
        if(ch < 65)
        {
            rest = 26 - key;
            ch += rest + key;
        }
    }
    data[iter] = ch;
    }
 return data;
}
