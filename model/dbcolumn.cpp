#include "dbcolumn.h"
#include "dbtable.h"
#include <QSqlDatabase>
#include <QSqlQuery>

DBColumn::DBColumn(QString name,
                   ColumnType type,
                   int length,
                   bool null,
                   QString defaultValue,
                   QString extraQuery,
                   bool pk,
                   DBTable *parent)
    : QObject(parent)
{
    this->m_name = name;
    this->m_type = type;
    this->m_length = length;
    this->m_null = null;
    this->m_defaultValue = defaultValue;
    this->m_extraQuery = extraQuery;
    this->m_pk = pk;
}

void DBColumn::check(DBColumn *compareColumn, const QString &connectionName)
{
    if (compareColumn == nullptr) {
        add(connectionName);
    }

    //Check name
    if (m_name != compareColumn->m_name) {
        rename(compareColumn->m_name, connectionName);
    }

    //Check type
    if (typeWithLength() != compareColumn->typeWithLength()) {
        updateType(compareColumn, connectionName);
    }
}

void DBColumn::add(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);
    query.exec(QString("ALTER TABLE %1 ADD COLUMN %2 %3 %4 %5")
                   .arg(parentTable()->name(),
                        name(),
                        typeWithLength(),
                        nullString(),
                        defaultValue() == "" ? "" : "DEFAULT " + defaultValue()));
}

void DBColumn::rename(QString newName, const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);

    query.exec(
        QString("ALTER TABLE %1 RENAME %2 TO %3").arg(parentTable()->name(), name(), newName));
}

void DBColumn::updateType(DBColumn *compareColumn, const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);

    //First take care of the default value...
    if (m_defaultValue != "") {
        query.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 SET DEFAULT %3")
                       .arg(parentTable()->name(), name(), compareColumn->defaultValue()));
    } else {
        query.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 DROP DEFAULT")
                       .arg(parentTable()->name(), name()));
    }

    //...Then change the type
    query.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 TYPE %3%4")
                   .arg(parentTable()->name(),
                        name(),
                        compareColumn->typeWithLength(),
                        compareColumn->extraQuery()));
}

DBTable *DBColumn::parentTable()
{
    return dynamic_cast<DBTable *>(parent());
}

bool DBColumn::pk() const
{
    return m_pk;
}

QString DBColumn::extraQuery() const
{
    return m_extraQuery;
}

QString DBColumn::defaultValue() const
{
    return m_defaultValue;
}

bool DBColumn::null() const
{
    return m_null;
}

int DBColumn::length() const
{
    return m_length;
}

ColumnType DBColumn::type() const
{
    return m_type;
}

QString DBColumn::name() const
{
    return m_name;
}

QString DBColumn::typeWithLength() const
{
    QString type;

    type = this->type();

    if (m_length > 0)
        type += QString("(%1)").arg(m_length);

    return type;
}

QString DBColumn::nullString() const
{
    QString null;

    if (m_null)
        null = "";
    else
        null = "NOT NULL";

    return null;
}
