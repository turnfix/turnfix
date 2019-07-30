#ifndef DISCIPLINEFIELDREPOSITORY_H
#define DISCIPLINEFIELDREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/disciplinefield.h"

class Discipline;

class DisciplineFieldRepository : public AbstractRepository<DisciplineField>
{
public:
    explicit DisciplineFieldRepository(EntityManager *em);

    QList<DisciplineField *> loadByDiscipline(Discipline *discipline);
};

#endif // DISCIPLINEFIELDREPOSITORY_H
