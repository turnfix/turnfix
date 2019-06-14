#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "db.h"
#include "model/settings/tfsettings.h"

DB* DB::instance = nullptr;

DB::DB(QString name) {
    settings = TFSettings::getInstance();
    this->name = name;
}

bool DB::establishConnection()
{
    bool successful;
    int typ;
    QSqlDatabase db;
    QString fileName;

    typ = settings->getDbTyp();

    if(typ == DB::SQLite) {
        fileName = settings->getDbFilename();

        //No filename entered
        if (fileName == "")
        {
            emit errorOccured("Kein Dateiname angegeben. Bitte in den Einstellungen die Datenbank auswählen.");
            return false;
        }

        if (this->name != "") {
            db = QSqlDatabase::addDatabase("QSQLITE", this->name);
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE");
        }
        db.setDatabaseName(fileName);
    } else if (typ == DB::PostgreSQL) {
        if (this->name != "") {
            db = QSqlDatabase::addDatabase("QPSQL", this->name);
        } else {
            db = QSqlDatabase::addDatabase("QPSQL");
        }
        db.setHostName(settings->getDbServer());
        db.setDatabaseName(settings->getDbDatabase());
        db.setUserName(settings->getDbUser());
        db.setPassword(settings->getDbPass());
    }

    successful = db.open();
    if (!successful) {
        emit errorOccured(db.lastError().text());
    }

    //Enable Foreign Keys for SQLite Database
    if (successful && typ == DB::SQLite)
    {
        QSqlQuery("PRAGMA foreign_keys=true");
    }

    return successful;
}

void DB::closeConnection()
{
    QSqlDatabase database;

    database = QSqlDatabase::database(this->name);
    database.close();
}

int DB::count(TFSqlQuery query)
{
    switch(settings->getDbTyp())
    {
    default:
        return query.size();
    case SQLite:
        int i=0;
        query.exec();
        while (query.next())
            i++;
        query.exec();
        return i;
    }
}

QSqlDatabase DB::internal()
{
    return QSqlDatabase::database(this->name);
}

void DB::emitError(QString errorMessage)
{
    emit errorOccured(errorMessage);
}

DB* DB::createInstance(QString name)
{
    return new DB(name);
}

DB* DB::getInstance()
{
    static QMutex mutex;
    if (!instance)
    {
        mutex.lock();

        if (!instance)
            instance = new DB;

        mutex.unlock();
    }
    return instance;
}

void DB::dropInstance()
{
    static QMutex mutex;
    mutex.lock();
    delete instance;
    instance = nullptr;
    mutex.unlock();
}
