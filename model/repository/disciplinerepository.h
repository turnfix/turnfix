#ifndef DISCIPLINEREPOSITORY_H
#define DISCIPLINEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/discipline.h"

class DisciplineRepository : public AbstractRepository<Discipline>
{
public:
    explicit DisciplineRepository(EntityManager *em);

    QList<Discipline *> loadAll();
};

#endif // DISCIPLINEREPOSITORY_H
