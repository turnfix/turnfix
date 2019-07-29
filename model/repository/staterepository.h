#ifndef STATEREPOSITORY_H
#define STATEREPOSITORY_H

#include "abstractrepository.h"

class State;

class StateRepository : public AbstractRepository<State>
{
public:
    explicit StateRepository(EntityManager *em);

    QList<State *> loadAll();
};

#endif // STATEREPOSITORY_H
