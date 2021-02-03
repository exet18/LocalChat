#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

class Database
{
public:
    Database();
    Database(QString host_name, QString database_name);

    void setDebugMode(bool mode){debug_Mode = mode;};

    void setHostName(QString host_name) {chat_data.setHostName(host_name);};
    void setDataBaseName(QString database_name) {chat_data.setDatabaseName(database_name);};

    bool open_connection();
    void close_connection();

    bool add_User(QString name, QString password);
    bool del_User(QString name);
    int check_password(QString name, QString password);

private:
    void initialize();
    bool create_new_database();
    bool exec_SQL(QString queryStr);
    int check_user_exist(QString name);
    void log(QString message);

    QSqlDatabase chat_data;
    bool debug_Mode = false;

};

#endif // DATABASE_H
