#ifndef EVENTREPOSITORY_H
#define EVENTREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/event.h"

class EventRepository : public AbstractRepository<Event>
{
public:
    explicit EventRepository(EntityManager *em);

    QList<Event *> loadAll();
    bool persist(Event *event) override;
};

#endif // EVENTREPOSITORY_H
