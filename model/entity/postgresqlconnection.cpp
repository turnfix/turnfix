#include "postgresqlconnection.h"
#include <QSqlError>

QString PostgreSQLConnection::server() const
{
    return m_server;
}

void PostgreSQLConnection::setServer(const QString &value)
{
    m_server = value;
}

QString PostgreSQLConnection::username() const
{
    return m_username;
}

void PostgreSQLConnection::setUsername(const QString &value)
{
    m_username = value;
}

QString PostgreSQLConnection::password() const
{
    return m_password;
}

void PostgreSQLConnection::setPassword(const QString &value)
{
    m_password = value;
}

QString PostgreSQLConnection::databaseName() const
{
    return m_databaseName;
}

void PostgreSQLConnection::setDatabaseName(const QString &value)
{
    m_databaseName = value;
}

bool PostgreSQLConnection::connect(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
    db.setHostName(m_server);
    db.setDatabaseName(m_databaseName);
    db.setUserName(m_username);
    db.setPassword(m_password);

    bool successful = db.open();
    if (!successful) {
        emit errorOccured(db.lastError().text());
    } else {
        m_databases.insert(connectionName, db);
    }

    return successful;
}
