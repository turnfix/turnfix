#include <QVariant>
#include "location.h"

QString Location::getName()
{
    return this->name;
}

QString Location::getCity()
{
    return this->city;
}

QString Location::getNameAndCity()
{
    return this->name + ", " + this->city;
}

QMap<int,Location*> Location::getLocationsWithIds(QList<int> ids)
{
    QMap<int,Location*> output;
    TFSqlQuery query;
    Location *location;

    if (ids.size() == 0)
        return output;

    query.prepare("SELECT * FROM tfx_wettkampforte WHERE int_wettkampforteid IN ("+TFSqlQuery::intListToString(ids)+")");
    query.exec();
    while (query.next())
    {
        location = new Location();
        location->setData(query);
        output.insert(location->id,location);
    }
    return output;
}

void Location::setData(TFSqlQuery query)
{
    this->id = query.value(0).toInt();
    this->name = query.value(1).toString();
    this->address = query.value(2).toString();
    this->zip = query.value(3).toString();
    this->city = query.value(4).toString();
}
