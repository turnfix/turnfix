#include "dbtable.h"
#include "dbcolumn.h"
#include "dbconstraint.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

DBTable::DBTable(QString name, QObject *parent) : QObject(parent) {
    m_name = name;
}

void DBTable::addColumn(QString property,
                        QString name,
                        ColumnType type,
                        int length,
                        bool null,
                        QString defaultValue,
                        QString extraQuery,
                        bool pk)
{
    DBColumn *column = new DBColumn(name, type, length, null, defaultValue, extraQuery, pk, this);
    m_columns.insert(property, column);
}

void DBTable::addContraint(QString name,
                           QString referenceTable,
                           QString fromField,
                           QString toField,
                           QString onUpdate,
                           QString onDelete)
{
    DBConstraint *constraint
        = new DBConstraint(name, referenceTable, fromField, toField, onUpdate, onDelete, this);
    m_constraints.append(constraint);
}

QString DBTable::name() const
{
    return m_name;
}

int DBTable::columnFKCount()
{
    return m_columns.size() + m_constraints.size();
}

DBColumn *DBTable::columnByProperty(const QString &propertName) const
{
    return m_columns.value(propertName);
}

void DBTable::check(const QString &connectionName)
{
    if (!exists(connectionName)) {
        create(connectionName);
        return;
    }

    QList<DBColumn *> compareColumns = existingColumns(connectionName);
    QList<DBConstraint *> compareConstraints = existingConstraints(connectionName);

    QList<DBColumn *> columns = m_columns.values();
    //Check if all columns exist in database
    for (int i = 0; i < columns.size(); i++) {
        DBColumn *column = columns.at(i);
        DBColumn *compareColumn = compareColumns.at(i);

        column->check(compareColumn, connectionName);

        emit columnsChecked(i + 1);
    }

    //Check contraints
    for (int i = 0; i < m_constraints.size(); i++) {
        DBConstraint *constraint = m_constraints.at(i);
        DBConstraint *compareConstraint = compareConstraints.at(i);

        constraint->check(compareConstraint, connectionName);

        emit columnsChecked(m_columns.size() + i + 1);
    }
}

void DBTable::create(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString driverName = db.driverName();
    QSqlQuery create(db);
    QString query;
    int pk;
    int i;

    query = "CREATE TABLE " + this->name() + " (";
    pk = -1;
    i = 0;

    foreach (DBColumn *column, m_columns) {
        query += column->name() + " ";
        if (column->pk()) {
            if (driverName == "QPSQL") {
                query += "serial";
            } else if (driverName == "QSQLITE") {
                query += "integer";
            } else {
                query += column->typeWithLength();
            }
        } else {
            query += column->typeWithLength();
        }
        if (!column->null())
            query += " NOT NULL";
        if (column->defaultValue() != "")
            query += " DEFAULT " + column->defaultValue();
        query += ",";
        if (column->pk())
            pk = i;
        i++;
    }
    // TODO reactivate
    //    if (pk != -1)
    //    {
    //        query += "CONSTRAINT pky_" + m_columns.at(pk)->name() + " PRIMARY KEY ("
    //                 + m_columns.at(pk)->name() + "),";
    //    }
    foreach (DBConstraint *constraint, m_constraints) {
        query += "CONSTRAINT " + constraint->name() + " FOREIGN KEY (" + constraint->fromField()
                 + ") ";
        query += "REFERENCES " + constraint->referenceTable() + " (" + constraint->toField() + ") ";
        query += "ON UPDATE " + constraint->onUpdate() + " ON DELETE " + constraint->onDelete()
                 + ",";
    }

    query = query.left(query.length()-1);
    query += ")";

    create.exec(query);
}

bool DBTable::exists(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString driverName = db.driverName();

    QSqlQuery query(db);
    if (driverName == "QPSQL") {
        query.prepare("SELECT tablename FROM pg_tables WHERE tablename = :name");
    } else if (driverName == "QSQLITE") {
        query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name= :name");
    } else {
        return false;
    }
    query.bindValue(":name", m_name);
    query.exec();

    return query.next();
}

QList<DBColumn *> DBTable::existingColumns(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString driverName = db.driverName();
    QList<DBColumn *> columns;

    QSqlQuery query(db);
    if (driverName == "QPSQL") {
        query.prepare(
            "SELECT (ordinal_position-1), "
            "column_name,data_type,is_nullable,column_default,character_maximum_length FROM "
            "information_schema.columns WHERE table_name= :name ORDER BY ordinal_position");
    } else if (driverName == "QSQLITE") {
        query.prepare("PRAGMA table_info(:name)");
    } else {
        return columns;
    }
    query.bindValue(":name", m_name);
    query.exec();

    while (query.next()) {
    }

    return columns;
}

QList<DBConstraint *> DBTable::existingConstraints(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString driverName = db.driverName();
    QList<DBConstraint *> constraints;

    QSqlQuery query(db);
    if (driverName == "QPSQL") {
        query.prepare(
            "SELECT DISTINCT ordinal_position, 0, ccu.table_name AS references_table, "
            "kcu.column_name, ccu.column_name AS references_field, rc.update_rule AS on_update, "
            "rc.delete_rule AS on_delete, match_option FROM information_schema.table_constraints "
            "tc LEFT JOIN information_schema.key_column_usage kcu ON tc.constraint_catalog = "
            "kcu.constraint_catalog AND tc.constraint_schema = kcu.constraint_schema AND "
            "tc.constraint_name = kcu.constraint_name LEFT JOIN "
            "information_schema.referential_constraints rc ON tc.constraint_catalog = "
            "rc.constraint_catalog AND tc.constraint_schema = rc.constraint_schema AND "
            "tc.constraint_name = rc.constraint_name LEFT JOIN "
            "information_schema.constraint_column_usage ccu ON rc.unique_constraint_catalog = "
            "ccu.constraint_catalog AND rc.unique_constraint_schema = ccu.constraint_schema AND "
            "rc.unique_constraint_name = ccu.constraint_name WHERE tc.table_name = :name AND "
            "tc.constraint_type='FOREIGN KEY'");
    } else if (driverName == "QSQLITE") {
        query.prepare("PRAGMA foreign_key_list(:name)");
    } else {
        return constraints;
    }
    query.bindValue(":name", m_name);
    query.exec();

    while (query.next()) {
    }

    return constraints;
}
