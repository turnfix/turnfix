#ifndef COMPETITIONDISCIPLINEREPOSITORY_H
#define COMPETITIONDISCIPLINEREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/competitiondiscipline.h"

class CompetitionDisciplineRepository : public AbstractRepository<CompetitionDiscipline>
{
public:
    explicit CompetitionDisciplineRepository(EntityManager *em);

    QList<CompetitionDiscipline *> fetchByCompetition(Competition *competition);
};

#endif // COMPETITIONDISCIPLINEREPOSITORY_H
