#include "abstractconnection.h"

AbstractConnection::AbstractConnection()
{
    uuid = QUuid::createUuid();
}

AbstractConnection::Type AbstractConnection::getType() const
{
    return type;
}

void AbstractConnection::setType(const Type &value)
{
    type = value;
}

QString AbstractConnection::getName() const
{
    return name;
}

void AbstractConnection::setName(const QString &value)
{
    name = value;
}

QUuid AbstractConnection::getUuid() const
{
    return uuid;
}
