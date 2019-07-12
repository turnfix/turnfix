#ifndef EVENTREPOSITORY_H
#define EVENTREPOSITORY_H

#include "abstractrepository.h"

class Event;

class EventRepository : public AbstractRepository<Event *>
{
public:
    QList<Event *> loadAll() override;
    void persist(Event *event) override;
    void remove(Event *event) override;
};

#endif // EVENTREPOSITORY_H
