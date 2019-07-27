#ifndef DBTABLE_H
#define DBTABLE_H

#include "enums.h"
#include <QObject>

class DBConstraint;
class DBColumn;

class DBTable : public QObject
{
    Q_OBJECT

public:
    explicit DBTable(QString m_name, QObject *parent = nullptr);

    void addColumn(QString name,
                   ColumnType type,
                   int length = 0,
                   bool null = true,
                   QString defaultValue = "",
                   QString extraQuery = "",
                   bool pk = false);
    void addContraint(QString name,
                      QString referenceTable,
                      QString fromField,
                      QString toField,
                      QString onUpdate,
                      QString onDelete);
    QString name();
    int columnFKCount();

    void check(const QString &connectionName);

private:
    void create(const QString &connectionName);
    bool exists(const QString &connectionName);
    QList<DBColumn *> existingColumns(const QString &connectionName);
    QList<DBConstraint *> existingConstraints(const QString &connectionName);

    QString m_name;
    QList<DBColumn *> m_columns;
    QList<DBConstraint *> m_constraints;

signals:
    void tableOK();
    void tablesDoesNotExist();
    void columnsChecked(int);

};

#endif // DBTABLE_H