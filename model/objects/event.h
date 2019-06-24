#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDate>
#include "model/database/tfsqlquery.h"

class Location;

class Event : public QObject
{
    Q_OBJECT
public:
    int getId();
    Location *getLocation();
    QString getName();
    QDate getStartDate();
    int getMainEventId();
    int getRound();
    bool isMultiRoundEvent();

    static QList<Event*> getAllEvents();

private:
    int id;
    int locationId;
    Location *location;
    QString name;
    QDate startDate;
    int mainEventId;
    int round;
    bool multiRoundEvent;

    void setData(TFSqlQuery query);

};

#endif // EVENT_H
