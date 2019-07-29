#ifndef DBCOLUMN_H
#define DBCOLUMN_H

#include "enums.h"
#include <QObject>

class DBTable;
class DBConstraint;

class DBColumn : public QObject
{
    Q_OBJECT
public:
    explicit DBColumn(QString name,
                      ColumnType type,
                      int length = 0,
                      bool null = true,
                      QString defaultValue = "",
                      QString extraQuery = "",
                      bool pk = false,
                      DBTable *parent = nullptr);

    QString name() const;
    ColumnType type() const;
    QString typeWithLength() const;
    int length() const;
    bool null() const;
    QString nullString() const;
    QString defaultValue() const;
    QString extraQuery() const;
    bool pk() const;

    void addContraint(
        QString name, QString referenceTable, QString toField, QString onUpdate, QString onDelete);
    QList<DBConstraint *> constraints() const;

    void check(DBColumn *compareColumn, const QString &connectionName);

private:
    void add(const QString &connectionName);
    void rename(QString newName, const QString &connectionName);
    void updateType(DBColumn *compareColumn, const QString &connectionName);

    DBTable *parentTable();

    QString m_name;
    ColumnType m_type;
    int m_length;
    bool m_null;
    QString m_defaultValue;
    QString m_extraQuery;
    bool m_pk;
    QList<DBConstraint *> m_constraints;
};

#endif // DBCOLUMN_H
