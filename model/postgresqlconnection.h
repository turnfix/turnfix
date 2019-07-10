#ifndef POSTGRESQLCONNECTION_H
#define POSTGRESQLCONNECTION_H

#include "abstractconnection.h"

class PostgreSQLConnection : public AbstractConnection
{
public:
    QString getServer() const;
    void setServer(const QString &value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getDatabaseName() const;
    void setDatabaseName(const QString &value);

private:
    QString server;
    QString username;
    QString password;
    QString databaseName;
};

#endif // POSTGRESQLCONNECTION_H
