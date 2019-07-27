#ifndef DIVISIONREPOSITORY_H
#define DIVISIONREPOSITORY_H

#include "abstractrepository.h"

class Division;

class DivisionRepository : public AbstractRepository<Division>
{
public:
    explicit DivisionRepository(EntityManager *em);

    void persist(Division *division);
    void remove(Division *division);
};

#endif // DIVISIONREPOSITORY_H
