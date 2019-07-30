#ifndef STATEREPOSITORY_H
#define STATEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/state.h"

class StateRepository : public AbstractRepository<State>
{
public:
    explicit StateRepository(EntityManager *em);

    QList<State *> loadAll();
};

#endif // STATEREPOSITORY_H
