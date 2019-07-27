#ifndef DISCIPLINEFIELDREPOSITORY_H
#define DISCIPLINEFIELDREPOSITORY_H

#include "abstractrepository.h"

class Discipline;
class DisciplineField;

class DisciplineFieldRepository : public AbstractRepository<DisciplineField>
{
public:
    explicit DisciplineFieldRepository(EntityManager *em);

    QList<DisciplineField *> loadByDiscipline(Discipline *discipline);
};

#endif // DISCIPLINEFIELDREPOSITORY_H
