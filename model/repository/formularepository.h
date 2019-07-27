#ifndef FORMULAREPOSITORY_H
#define FORMULAREPOSITORY_H

#include "abstractrepository.h"

class Formula;

class FormulaRepository : public AbstractRepository<Formula>
{
public:
    explicit FormulaRepository(EntityManager *em);

    QList<Formula *> loadAll();
};

#endif // FORMULAREPOSITORY_H
