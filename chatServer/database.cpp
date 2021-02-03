#include "database.h"

Database::Database()
{
    initialize();
}

Database::Database(QString host_name, QString database_name)
{
    initialize();
    setHostName(host_name);
    setDataBaseName(database_name);
}

bool Database::open_connection()
{
    if(chat_data.open()){
        qDebug() << "Opened database at " << chat_data.hostName();
        bool ok = create_new_database();
        if(!ok){
            qDebug() << "Error creating database! Opened empty file";
        }
        return true;
    } else {
        qDebug() << chat_data.lastError().databaseText();
        return false;
    }
}

void Database::close_connection()
{
    chat_data.close();
}

bool Database::add_User(QString name, QString password)
{
    if(check_user_exist(name) != 0){
        return false;
    }
    QString queryStr = QString("INSERT INTO users(login,password) VALUES ('%1', '%2');")
            .arg(name,password);
    if(!exec_SQL(queryStr)){
        log("Error:Cannot create user!");
        return false;
    }
    return true;

}

bool Database::del_User(QString name)             //!!!check this function!!!
{
    if(check_user_exist(name) != 2){
        return false;
    }
    QString queryStr = QString("DELETE FROM users WHERE login='" + name + "';");
    if(!exec_SQL(queryStr)){
        log("Error:Cannot delete user!");
        return false;
    }
    qDebug() << "User " << name << " was succesfully deleted from database";
    return true;
}

int Database::check_password(QString name, QString password)
{
    QSqlQuery query;
    QString queryStr = "SELECT password FROM users WHERE login='" + name + "';";
    bool ok = query.exec(queryStr);
    if(!ok){
        log("Error executting query: " + query.lastError().databaseText());
        return 1;
    }

    ok = query.next();

    if(!ok){
        log("Error: no such user!");
        return 2;
    }

    QString passwordFromDatabase = query.value("password").toString();

    if(password != passwordFromDatabase){
        log("Wrong password!");
        return 3;
    }

    log("User" + name + "Password correct");
    return 0;
}

void Database::initialize()
{
    chat_data =  QSqlDatabase::addDatabase("QSQLITE");
}

bool Database::create_new_database()
{
    QString querty_str = "CREATE TABLE IF NOT EXISTS users (login TEXT,password TEXT);";
    if(!exec_SQL(querty_str)){
        log("Error: Can't create init table!");
        return false;
    }
    return true;
}

bool Database::exec_SQL(QString queryStr)
{
    QSqlQuery query;
    bool ok = query.exec(queryStr); // checking query on succesful operation
    if(!ok){
        qDebug() << "Database error occured:" << query.lastError().databaseText();
    }
    return ok;
}

int Database::check_user_exist(QString name)
{
    QSqlQuery query;
    QString quertyStr = "SELECT password FROM users WHERE login='" + name + "';";
    bool ok = query.exec(quertyStr);
    if(!ok){
        log("Error checking user exist:" + query.lastError().databaseText());
        return 1;
    }

    if(query.next()){
        log("Error: User " + name + " already exists!");
        return 2;
    }

    log("Username " + name + "is available!");
    return 0;
}

void Database::log(QString message)
{
    if(debug_Mode){
        qDebug() << message;
    }
}
