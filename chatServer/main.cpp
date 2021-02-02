#include <QCoreApplication>
#include "servak.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Servak *servak = new Servak();

    int port = 8080;

    bool ok = servak->listen(QHostAddress::Any, port);

    if (ok) {
        qDebug() << "Server started at: " << servak->serverAddress().toString() << "\nPort: " << servak->serverPort();
    } else {
        qDebug() << "Server failed to start: " << servak->errorString();
    }

    return a.exec();
}
