#ifndef CLUBREPOSITORY_H
#define CLUBREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/club.h"

class Club;

class ClubRepository : public AbstractRepository<Club>
{
public:
    explicit ClubRepository(EntityManager *em);

    QList<Club *> loadAll();
};

#endif // CLUBREPOSITORY_H
