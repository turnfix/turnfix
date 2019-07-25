#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QUuid>

class QUuid;
class DBTable;

class AbstractConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    enum Type { SQLite, PostgreSQL };

    explicit AbstractConnection(QObject *parent = nullptr);
    explicit AbstractConnection(const QString &id, QObject *parent = nullptr);

    QUuid uuid() const;

    QString name() const;
    void setName(const QString &value);

    virtual bool connect(const QString &connectionName) = 0;
    void close(const QString &connectionName);

    static const DBTable *mapping();

protected:
    QMap<QString, QSqlDatabase> m_databases;

private:
    QUuid m_uuid;
    QString m_name;

signals:
    void errorOccured(const QString &message);
};

#endif // ABSTRACTCONNECTION_H
