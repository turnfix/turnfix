#include "statusrepository.h"

StatusRepository::StatusRepository(EntityManager *em)
    : AbstractRepository<Status>(em)
{
    
}

QList<Status *> StatusRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Status> qb;
    qb.select(Status::staticMetaObject, Status::mapping());
    qb.orderBy("Status", "name");

    QList<Status *> output = qb.query(db);

    return output;
}
