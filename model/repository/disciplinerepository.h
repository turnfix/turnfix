#ifndef DISCIPLINEREPOSITORY_H
#define DISCIPLINEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/discipline.h"

class DisciplineRepository : public AbstractRepository<Discipline>
{
public:
    explicit DisciplineRepository(EntityManager *em);

    QList<Discipline *> loadAll();
    QList<Discipline *> loadByGender(bool women, bool men);
};

#endif // DISCIPLINEREPOSITORY_H
