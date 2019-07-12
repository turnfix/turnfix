#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "abstractconnection.h"

class SQLiteConnection : public AbstractConnection
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)

public:
    explicit SQLiteConnection(QObject *parent = nullptr)
        : AbstractConnection(parent)
    {}
    explicit SQLiteConnection(const QString &id, QObject *parent = nullptr)
        : AbstractConnection(id, parent)
    {}

    QString fileName() const;
    void setFileName(const QString &value);

    bool connect(const QString &connectionName) override;

private:
    QString m_fileName;
};

#endif // SQLITECONNECTION_H
