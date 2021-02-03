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
    if(key > 0 and key < 22)
    {
        //QString password = ui->password->text();
        int port = ui->sb_port->value();
        socket->connectToHost(ip, port);
        //password = password_encryption(password);
        socket->write(data_encryption(nickname + " has connected to server." + '\n').toUtf8());
        //socket->write(password.toUtf8());
    }
}

void Chat::connected_to_server()
{
        ui->stackedWidget->setCurrentWidget(ui->messages);
        ui->statusbar->showMessage("Connected to " " "+ ui->e_address->text());
}

void Chat::send()
{
    QString message = ui->e_nickname->text();
    message += ": ";
    message += ui->send_message->toPlainText();
    message = data_encryption(message);
    socket->write(message.toUtf8());
    ui->send_message->clear();

}

void Chat::recieve()
{
    QString message = QString::fromUtf8(socket->readAll()).toLatin1();
    message = data_decryption(message);
    ui->text_messages->setHtml(ui->text_messages->toHtml() + message);
}

void Chat::disconnect()
{
    socket->disconnectFromHost();
    ui->statusbar->showMessage("Disconnected from" " " + ui->e_address->text());
    ui->stackedWidget->setCurrentWidget(ui->connection_page);
}


/*QString Chat::password_encryption(QString data)
    {
        for(int iter = 0; iter < data.length(); iter++){
            char sym = QString(data).at(iter).toLatin1();
            sym += 4;
            data.replace(iter,1,sym);
        }
        return data;
    }*/

QString Chat::data_encryption(QString data)
{
    /*for(int iter = 0; iter < data.length(); iter++){
            char ch = QString(data).at(iter).toLatin1();

            if(ch >= 'a' && ch <= 'z')
            {
                ch = ch + key;

                if(ch > 'z'){
                    ch = ch - 'z' + 'a' - 1;
                }

                data.replace(iter,1,ch);
            }
            else if(ch >= 'A' && ch <= 'Z'){
                ch = ch + key;

                if(ch > 'Z'){
                    ch = ch - 'Z' + 'A' - 1;
                }

                data.replace(iter,1,ch);
            }
    }
    */
    for(int iter = 0; iter < data.length(); iter++){
           char sym = QString(data).at(iter).toLatin1();
           if(sym >= 'a' && sym <= 'z'){
               if(sym+key > 'z'){
                   sym -= 18;
               }
               else{
                   sym += key;
               }
           }
           else if(sym >= 'A' && sym <= 'Z'){
               if(sym+key > 'Z'){
                   sym -= 18;
               }
               else{
                   sym += key;
               }
           }
           data.replace(iter,1,sym);
       }
       return data;
}

QString Chat::data_decryption(QString data)
{
    /*for(int iter = 0; iter < data.length(); iter++){
            char ch = QString(data).at(iter).toLatin1();

            if(ch >= 'a' && ch <= 'z'){
                ch = ch - key;

                if(ch < 'a'){
                    ch = ch + 'z' - 'a' + 1;
                }

               data.replace(iter,1,ch);
            }
            else if(ch >= 'A' && ch <= 'Z'){
                ch = ch - key;

                if(ch < 'A'){
                    ch = ch + 'Z' - 'A' + 1;
                }

                data.replace(iter,1,ch);
            }
        }
        */
    for(int iter = 0; iter < data.length(); iter++){
            char sym = QString(data).at(iter).toLatin1();
            if(sym >= 'a' && sym <= 'z'){
                if(sym-key < 'a'){
                    sym += 18;
                }
                else{
                    sym -= key;
                }
            }
            else if(sym >= 'A' && sym <= 'Z'){
                if(sym-key < 'A'){
                    sym += 18;
                }
                else{
                    sym -= key;
                }
            }
            data.replace(iter,1,sym);
        }
        return data;

}
