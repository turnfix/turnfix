#ifndef DISCIPLINEREPOSITORY_H
#define DISCIPLINEREPOSITORY_H

#include "abstractrepository.h"

class Discipline;

class DisciplineRepository : public AbstractRepository<Discipline>
{
public:
    explicit DisciplineRepository(EntityManager *em);

    QList<Discipline *> loadAll();
    void persist(Discipline *discipline) override;
    void remove(Discipline *discipline) override;
};

#endif // DISCIPLINEREPOSITORY_H
