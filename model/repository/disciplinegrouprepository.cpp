#include "disciplinegrouprepository.h"
#include "model/entitymanager.h"

DisciplineGroupRepository::DisciplineGroupRepository(EntityManager *em)
    : AbstractRepository<DisciplineGroup>(em)
{}

QList<DisciplineGroup *> DisciplineGroupRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<DisciplineGroup> qb;
    qb.select(DisciplineGroup::staticMetaObject, DisciplineGroup::mapping());
    qb.orderBy("DisciplineGroup", "name");

    QList<DisciplineGroup *> output = qb.query(db);

    return output;
}
