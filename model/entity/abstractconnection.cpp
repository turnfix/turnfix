#include "abstractconnection.h"

AbstractConnection::AbstractConnection(QObject *parent)
    : QObject(parent)
{
    m_uuid = QUuid::createUuid();
    m_name = "Neue Verbindung";
}

AbstractConnection::AbstractConnection(const QString &id, QObject *parent)
    : QObject(parent)
{
    m_uuid = QUuid::fromString(id);
}

QString AbstractConnection::name() const
{
    return m_name;
}

void AbstractConnection::setName(const QString &value)
{
    m_name = value;
}

void AbstractConnection::close(const QString &connectionName)
{
    if (!m_databases.contains(connectionName)) {
        return;
    }

    QSqlDatabase database = m_databases.value(connectionName);
    database.close();
}

const DBTable *AbstractConnection::mapping()
{
    return nullptr;
}

QUuid AbstractConnection::uuid() const
{
    return m_uuid;
}
