#ifndef SPORTREPOSITORY_H
#define SPORTREPOSITORY_H

#include "abstractrepository.h"

class Sport;

class SportRepository : public AbstractRepository<Sport>
{
public:
    explicit SportRepository(EntityManager *em);

    QList<Sport *> loadAll();
};

#endif // SPORTREPOSITORY_H
