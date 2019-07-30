#ifndef DBTABLE_H
#define DBTABLE_H

#include "enums.h"
#include <QMap>
#include <QObject>

class DBConstraint;
class DBColumn;

class DBTable : public QObject
{
    Q_OBJECT

public:
    explicit DBTable(QString name, QObject *parent = nullptr);

    DBColumn *addColumn(QString property,
                        QString name,
                        ColumnType type,
                        int length = 0,
                        bool null = true,
                        QString defaultValue = "",
                        QString extraQuery = "",
                        bool pk = false);

    QString name() const;
    DBColumn *columnByProperty(const QString &propertName) const;
    QList<DBConstraint *> existingConstraints(const QString &connectionName);

    void check(const QString &connectionName);

private:
    void create(const QString &connectionName);
    bool exists(const QString &connectionName);
    QList<DBColumn *> existingColumns(const QString &connectionName);

    QString m_name;
    QMap<QString, DBColumn *> m_columns;

signals:
    void tableOK();
    void tablesDoesNotExist();
    void columnsChecked(int);

};

#endif // DBTABLE_H
