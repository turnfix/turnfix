#ifndef CLUBREPOSITORY_H
#define CLUBREPOSITORY_H

#include "abstractrepository.h"

class Club;

class ClubRepository : public AbstractRepository<Club>
{
public:
    explicit ClubRepository(EntityManager *em);

    QList<Club *> loadAll();
    void persist(Club *club);
    void remove(Club *club);
};

#endif // CLUBREPOSITORY_H
