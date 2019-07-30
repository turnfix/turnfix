#include "penaltyrepository.h"

PenaltyRepository::PenaltyRepository(EntityManager *em)
    : AbstractRepository<Penalty>(em)
{

}

QList<Penalty *> PenaltyRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Penalty> qb;
    qb.select(Penalty::staticMetaObject, Penalty::mapping());
    qb.orderBy("Penalty", "name");

    QList<Penalty *> output = qb.query(db);

    return output;
}
