#include "divisionrepository.h"
#include "model/entitymanager.h"

DivisionRepository::DivisionRepository(EntityManager *em)
    : AbstractRepository<Division>(em)
{}

QList<Division *> DivisionRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Division> qb;
    qb.select(Division::staticMetaObject, Division::mapping());
    qb.orderBy("Division", "name");

    QList<Division *> output = qb.query(db);

    return output;
}
