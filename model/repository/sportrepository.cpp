#include "sportrepository.h"
#include "model/entity/sport.h"
#include "model/entitymanager.h"

SportRepository::SportRepository(EntityManager *em)
    : AbstractRepository<Sport>(em)
{}

QList<Sport *> SportRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Sport> qb;
    qb.select(Sport::staticMetaObject, Sport::mapping());
    qb.orderBy("Sport", "name");

    QList<Sport *> output = qb.query(db);

    return output;
}
