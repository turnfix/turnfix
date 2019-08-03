#include "competitionrepository.h"
#include "model/entity/event.h"
#include "model/entitymanager.h"

CompetitionRepository::CompetitionRepository(EntityManager *em)
    : AbstractRepository<Competition>(em)
{}

QList<Competition *> CompetitionRepository::fetchByEvent(Event *event)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QueryBuilder<Competition> qb;
    qb.select(Competition::staticMetaObject, Competition::mapping());
    qb.join(Division::staticMetaObject, Division::mapping(), "Competition", "division", "divisionId");
    qb.where("Competition", "eventId", event->id());
    qb.orderBy("Competition", "number");

    QList<Competition *> output = qb.query(db);

    return output;
}

Competition *CompetitionRepository::fetchByNumber(Event *, const QString &)
{
    return nullptr;
}
