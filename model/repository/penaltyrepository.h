#ifndef PENALTYREPOSITORY_H
#define PENALTYREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/penalty.h"

class PenaltyRepository : public AbstractRepository<Penalty>
{
public:
    explicit PenaltyRepository(EntityManager *em);

    QList<Penalty *> loadAll();
};

#endif // PENALTYREPOSITORY_H
