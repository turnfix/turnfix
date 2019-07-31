#include "athleterepository.h"
#include "model/entity/club.h"

AthleteRepository::AthleteRepository(EntityManager *em)
    : AbstractRepository<Athlete>(em)
{}

QList<Athlete *> AthleteRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Athlete> qb;
    qb.select(Athlete::staticMetaObject, Athlete::mapping());
    qb.join(Club::staticMetaObject, Club::mapping(), "Athlete", "club", "clubId");
    qb.orderBy("Athlete", "lastName");
    qb.orderBy("Athlete", "firstName");

    QList<Athlete *> output = qb.query(db);

    return output;
}
