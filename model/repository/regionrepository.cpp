#include "regionrepository.h"
#include "model/entity/state.h"

RegionRepository::RegionRepository(EntityManager *em)
    : AbstractRepository<Region>(em)
{}

QList<Region *> RegionRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Region> qb;
    qb.select(Region::staticMetaObject, Region::mapping());
    qb.join(State::staticMetaObject, State::mapping(), "Region", "state", "stateId");
    qb.orderBy("Region", "name");

    QList<Region *> output = qb.query(db);

    return output;
}
