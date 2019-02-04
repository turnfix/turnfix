#ifndef EVENT_H
#define EVENT_H

#include "tfobject.h"
#include <QDate>

class Location;

class Event : public TFObject
{
    Q_OBJECT
public:
    int getId();
    Location *getLocation();
    QString getName();
    QDate getStartDate();

    static QList<Event*> getAllEvents();

private:
    int id;
    int locationId;
    Location *location;
    QString name;
    QDate startDate;

    void setData(TFSqlQuery query);

};

#endif // EVENT_H
