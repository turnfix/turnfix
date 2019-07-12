#include "dbconstraint.h"
#include "dbtable.h"
#include <QSqlDatabase>
#include <QSqlQuery>

DBConstraint::DBConstraint(QString name,
                           QString referenceTable,
                           QString fromField,
                           QString toField,
                           QString onUpdate,
                           QString onDelete,
                           DBTable *parent)
    : QObject(parent)
{
    this->m_name = name;
    this->m_referenceTable = referenceTable;
    this->m_fromField = fromField;
    this->m_toField = toField;
    this->m_onUpdate = onUpdate;
    this->m_onDelete = onDelete;
}

QString DBConstraint::name()
{
    return this->m_name;
}

QString DBConstraint::referenceTable()
{
    return this->m_referenceTable;
}

QString DBConstraint::fromField()
{
    return this->m_fromField;
}

QString DBConstraint::toField()
{
    return this->m_toField;
}

QString DBConstraint::onUpdate()
{
    return this->m_onUpdate;
}

QString DBConstraint::onDelete()
{
    return this->m_onDelete;
}

void DBConstraint::check(DBConstraint *compareConstraint, const QString &connectionName)
{
    if (compareConstraint == nullptr) {
        create(connectionName);
    }
}

void DBConstraint::create(const QString &connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);
    //Create constraint
    query.exec(QString("ALTER TABLE %1 ADD CONSTRAINT %2 FOREIGN KEY (%3) REFERENCES "
                       "%4 (%5) ON UPDATE %6 ON DELETE %7")
                   .arg(parentTable()->name(),
                        name(),
                        fromField(),
                        referenceTable(),
                        toField(),
                        onUpdate(),
                        onDelete()));
}

DBTable *DBConstraint::parentTable()
{
    return dynamic_cast<DBTable *>(parent());
}
