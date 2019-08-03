#include "competitiondisciplinerepository.h"
#include "model/entity/competitiondiscipline.h"
#include "model/entitymanager.h"

CompetitionDisciplineRepository::CompetitionDisciplineRepository(EntityManager *em)
    : AbstractRepository<CompetitionDiscipline>(em)
{
    
}

QList<CompetitionDiscipline *> CompetitionDisciplineRepository::fetchByCompetition(
    Competition *competition)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QueryBuilder<CompetitionDiscipline> qb;
    qb.select(CompetitionDiscipline::staticMetaObject, CompetitionDiscipline::mapping());
    qb.where("CompetitionDiscipline", "competitionId", competition->id());
    qb.orderBy("CompetitionDiscipline", "sort");

    QList<CompetitionDiscipline *> output = qb.query(db);

    for (auto competitionDiscipline : output) {
        competitionDiscipline->setCompetition(competition);
    }

    return output;
}
