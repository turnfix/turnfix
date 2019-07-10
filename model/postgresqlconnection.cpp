#include "postgresqlconnection.h"

QString PostgreSQLConnection::getServer() const
{
    return server;
}

void PostgreSQLConnection::setServer(const QString &value)
{
    server = value;
}

QString PostgreSQLConnection::getUsername() const
{
    return username;
}

void PostgreSQLConnection::setUsername(const QString &value)
{
    username = value;
}

QString PostgreSQLConnection::getPassword() const
{
    return password;
}

void PostgreSQLConnection::setPassword(const QString &value)
{
    password = value;
}

QString PostgreSQLConnection::getDatabaseName() const
{
    return databaseName;
}

void PostgreSQLConnection::setDatabaseName(const QString &value)
{
    databaseName = value;
}
