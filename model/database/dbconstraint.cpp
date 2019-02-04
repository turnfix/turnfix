#include "dbconstraint.h"

DBConstraint::DBConstraint(QString name, QString referenceTable, QString fromField,
                           QString toField, QString onUpdate,
                           QString onDelete, QObject *parent) :
    QObject(parent)
{
    this->name = name;
    this->referenceTable = referenceTable;
    this->fromField = fromField;
    this->toField = toField;
    this->onUpdate = onUpdate;
    this->onDelete = onDelete;
}

QString DBConstraint::getName()
{
    return this->name;
}

QString DBConstraint::getReferenceTable()
{
    return this->referenceTable;
}

QString DBConstraint::getFromField()
{
    return this->fromField;
}

QString DBConstraint::getToField()
{
    return this->toField;
}

QString DBConstraint::getOnUpdate()
{
    return this->onUpdate;
}

QString DBConstraint::getOnDelete()
{
    return this->onDelete;
}
