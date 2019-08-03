#include "disciplinepositionrepository.h"
#include "model/entity/competition.h"
#include "model/entity/competitiondiscipline.h"

DisciplinePositionRepository::DisciplinePositionRepository(EntityManager *em)
    : AbstractRepository<DisciplinePosition>(em)
{
    
}

QList<DisciplinePosition *> DisciplinePositionRepository::loadByCompetition(
    Competition *m_competition)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<DisciplinePosition> qb;
    qb.select(DisciplinePosition::staticMetaObject, DisciplinePosition::mapping());
    qb.join(CompetitionDiscipline::staticMetaObject,
            CompetitionDiscipline::mapping(),
            "DisciplinePosition",
            "competitionDiscipline",
            "competitionDisciplineId");
    qb.where("CompetitionDiscipline", "competitionId", m_competition->id());

    QList<DisciplinePosition *> output = qb.query(db);

    return output;
}
