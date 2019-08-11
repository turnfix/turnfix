#ifndef DISCIPLINEPOSITIONREPOSITORY_H
#define DISCIPLINEPOSITIONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/disciplineposition.h"

class Competition;

class DisciplinePositionRepository : public AbstractRepository<DisciplinePosition>
{
public:
    explicit DisciplinePositionRepository(EntityManager *em);

    QList<DisciplinePosition *> loadByCompetition(Competition *m_competition);
};

#endif // DISCIPLINEPOSITIONREPOSITORY_H
