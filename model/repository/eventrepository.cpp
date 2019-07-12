#include "eventrepository.h"
#include "model/entity/event.h"
#include <QSqlQuery>

void EventRepository::persist(Event *event) {}

void EventRepository::remove(Event *event) {}

QList<Event *> EventRepository::loadAll()
{
    QList<Event *> output;
    //    QList<int> locationIds;
    //    QMap<int, Location *> locations;
    //    Event *event;
    //    QSqlQuery query;

    //    query.prepare(
    //        "SELECT int_veranstaltungenid, int_wettkampforteid, int_hauptwettkampf, int_runde, "
    //        "bol_rundenwettkampf, var_name, dat_von FROM tfx_veranstaltungen ORDER BY dat_von DESC");
    //    query.exec();
    //    while (query.next()) {
    //        event = new Event();

    //        // event->setData(query);
    //        if (!locationIds.contains(query.value(1).toInt()))
    //            locationIds.append(query.value(1).toInt());
    //        output.append(event);
    //    }

    //    locations = Location::getLocationsWithIds(locationIds);

    //    foreach (event, output) {
    //        event->location = locations.value(event->locationId);
    //    }

    //    this->id = query.value(0).toInt();
    //    this->locationId = query.value(1).toInt();
    //    this->mainEventId = query.value(2).toInt();
    //    this->round = query.value(3).toInt();
    //    this->multiRoundEvent = query.value(4).toBool();
    //    this->name = query.value(5).toString();
    //    this->startDate = query.value(6).toDate();

    return output;
}
