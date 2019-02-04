#include <QVariant>
#include "event.h"
#include "location.h"

int Event::getId()
{
    return this->id;
}

Location* Event::getLocation()
{
    return this->location;
}

QString Event::getName()
{
    return this->name;
}

QDate Event::getStartDate()
{
    return this->startDate;
}

QList<Event*> Event::getAllEvents()
{
    QList<Event*> output;
    QList<int> locationIds;
    QMap<int,Location*> locations;
    Event *event;
    TFSqlQuery query;

    query.prepare("SELECT * FROM tfx_veranstaltungen ORDER BY dat_von DESC");
    query.exec();
    while (query.next())
    {
        event = new Event;
        event->setData(query);
        if (!locationIds.contains(query.value(1).toInt()))
            locationIds.append(query.value(1).toInt());
        output.append(event);
    }

    locations = Location::getLocationsWithIds(locationIds);

    foreach (event, output)
    {
        event->location = locations.value(event->locationId);
    }

    return output;
}

void Event::setData(TFSqlQuery query)
{
    this->id = query.value(0).toInt();
    this->locationId = query.value(1).toInt();
    this->name = query.value(6).toString();
    this->startDate = query.value(8).toDate();
}
