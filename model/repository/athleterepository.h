#ifndef ATHLETEREPOSITORY_H
#define ATHLETEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/athlete.h"

class AthleteRepository : public AbstractRepository<Athlete>
{
public:
    explicit AthleteRepository(EntityManager *em);

    QList<Athlete *> loadAll();
};

#endif // ATHLETEREPOSITORY_H
