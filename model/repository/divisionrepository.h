#ifndef DIVISIONREPOSITORY_H
#define DIVISIONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/division.h"

class DivisionRepository : public AbstractRepository<Division>
{
public:
    explicit DivisionRepository(EntityManager *em);
};

#endif // DIVISIONREPOSITORY_H
