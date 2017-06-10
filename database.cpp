#include "database.h"
#include <QDebug>
#include <QFileInfo>

Database::Database()
{
    path = "/Users/chelalimohamed-tayeb/Documents/QtProject/Gestion_bibliotheque/librery.sqlite";
    //path = "C:/Users/Yahia/Documents/Gestion_bibliotheque/librery.sqlite";
}

bool Database::dbConnect(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()){
        qDebug() << "error connection ";
        return false;
    }else{
        qDebug() << "connected ";
        return true;
    }
}

void Database::dbclose(){

    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);

}

