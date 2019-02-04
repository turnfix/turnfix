#include "dbcolumn.h"

DBColumn::DBColumn(QString name, QString type, int length,
                   bool null, QString defaultValue, QString extraQuery, bool pk, QObject *parent) :
QObject(parent)
{
    this->name = name;
    this->type = type;
    this->length = length;
    this->null = null;
    this->defaultValue = defaultValue;
    this->extraQuery = extraQuery;
    this->pk = pk;
}

QString DBColumn::getName()
{
    return this->name;
}

QString DBColumn::getType()
{
    return this->type;
}

QString DBColumn::getTypeWithLength()
{
    QString type;

    type = this->getType();

    if (this->length > 0)
        type += QString("(%1)").arg(this->length);

    return type;
}

int DBColumn::getLength()
{
    return this->length;
}

bool DBColumn::getNull()
{
    return this->null;
}

QString DBColumn::getNullString()
{
    QString null;

    if (this->null)
        null = "";
    else
        null = "NOT NULL";

    return null;
}

QString DBColumn::getDefaultValue()
{
    return this->defaultValue;
}

QString DBColumn::getExtraQuery()
{
    return this->extraQuery;
}

bool DBColumn::getPK()
{
    return this->pk;
}
