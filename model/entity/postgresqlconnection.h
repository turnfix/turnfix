#ifndef POSTGRESQLCONNECTION_H
#define POSTGRESQLCONNECTION_H

#include "abstractconnection.h"

class PostgreSQLConnection : public AbstractConnection
{
    Q_OBJECT
    Q_PROPERTY(QString server READ server WRITE setServer)
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)

public:
    explicit PostgreSQLConnection(QObject *parent = nullptr)
        : AbstractConnection(parent)
    {}
    explicit PostgreSQLConnection(const QString &id, QObject *parent = nullptr)
        : AbstractConnection(id, parent)
    {}

    QString server() const;
    void setServer(const QString &value);

    QString username() const;
    void setUsername(const QString &value);

    QString password() const;
    void setPassword(const QString &value);

    QString databaseName() const;
    void setDatabaseName(const QString &value);

    bool connect(const QString &connectionName) override;

private:
    QString m_server;
    QString m_username;
    QString m_password;
    QString m_databaseName;
};

#endif // POSTGRESQLCONNECTION_H
