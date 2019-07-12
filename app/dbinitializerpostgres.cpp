#include "dbinitializerpostgres.h"
#include <QSqlError>
#include <QSqlQuery>

DBInitializerPostgres::DBInitializerPostgres(QObject *parent) : QThread(parent) {}

void DBInitializerPostgres::run()
{
    QSqlDatabase db;

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(server);
    db.setUserName(adminUser);
    db.setPassword(adminPass);

    if (!db.open()) {
        emit statusUpdate(0,false,tr("Die Datenbank meldet: %1").arg(db.lastError().text()));
        return;
    }

    QSqlQuery query;

    emit statusUpdate(0,true);

    query.exec("SELECT usename FROM pg_user WHERE usename='"+tfUser+"'");
    if (query.size()==0) {
        query.exec("CREATE ROLE "+tfUser+" LOGIN ENCRYPTED PASSWORD '"+tfPass+"'");
        if (query.numRowsAffected() == -1) {
            emit statusUpdate(1,false);
            return;
        } else {
            emit statusUpdate(1,true);
        }
    } else {
        emit statusUpdate(1,true,tr("Der Benutzer %1 existiert bereits. Sie müssen später zum Login das Passwort des Benutzers angeben.").arg(tfUser));
    }
    query.exec("SELECT datname FROM pg_database WHERE datname='"+databaseName+"'");
    if (query.size()==0) {
        query.exec("CREATE DATABASE "+databaseName+" WITH OWNER "+tfUser);
        if (query.numRowsAffected() == -1) {
            emit statusUpdate(2,false);
            return;
        } else {
            emit statusUpdate(2,true);
        }
    } else {
        emit statusUpdate(2,true,tr("Die Datenbank %1 existiert bereits. Eventuell hat der TurnFix-Benutzer keine Rechte auf die Daten zuzugreifen. Wählen Sie einen anderen Namen wenn es Probleme gibt.").arg(databaseName));
    }
    db.close();

    db.setHostName(server);
    db.setUserName(tfUser);
    db.setPassword(tfPass);
    db.setDatabaseName(databaseName);
    if (db.open()) {
        emit statusUpdate(3,true);
    } else {
        emit statusUpdate(3,false,tr("Die Datenbank meldet: %1").arg(db.lastError().text()));
    }
}


void DBInitializerPostgres::setData(QString adminUser, QString adminPass, QString tfUser,
                                    QString tfPass, QString databaseName, QString server)
{
    this->adminUser = adminUser;
    this->adminPass = adminPass;
    this->tfUser = tfUser;
    this->tfPass = tfPass;
    this->databaseName = databaseName;
    this->server = server;
}
