#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <QString>
#include <QUuid>

class QUuid;

class AbstractConnection
{
public:
    enum Type { SQLite, PostgreSQL };

    explicit AbstractConnection();

    QUuid getUuid() const;

    Type getType() const;
    void setType(const Type &value);

    QString getName() const;
    void setName(const QString &value);

private:
    QUuid uuid;
    Type type;
    QString name;
};

#endif // ABSTRACTCONNECTION_H
