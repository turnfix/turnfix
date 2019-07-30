#ifndef COMPETITIONREPOSITORY_H
#define COMPETITIONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/competition.h"

class Event;

class CompetitionRepository : public AbstractRepository<Competition>
{
public:
    explicit CompetitionRepository(EntityManager *em);

    QList<Competition *> fetchByEvent(Event *event);
    Competition *fetchByNumber(Event *event, const QString &number);
};

#endif // COMPETITIONREPOSITORY_H
