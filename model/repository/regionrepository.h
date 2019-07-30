#ifndef REGIONREPOSITORY_H
#define REGIONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/region.h"

class RegionRepository : public AbstractRepository<Region>
{
public:
    explicit RegionRepository(EntityManager *em);

    QList<Region *> loadAll();
};

#endif // REGIONREPOSITORY_H
