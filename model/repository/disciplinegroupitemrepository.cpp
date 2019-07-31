#include "disciplinegroupitemrepository.h"
#include "model/entity/discipline.h"
#include "model/entity/disciplinegroup.h"
#include "model/entity/sport.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"

DisciplineGroupItemRepository::DisciplineGroupItemRepository(EntityManager *em)
    : AbstractRepository<DisciplineGroupItem>(em)
{

}

QList<DisciplineGroupItem *> DisciplineGroupItemRepository::loadByGroup(DisciplineGroup *group)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<DisciplineGroupItem> qb;
    qb.select(DisciplineGroupItem::staticMetaObject, DisciplineGroupItem::mapping());
    qb.join(Discipline::staticMetaObject,
            Discipline::mapping(),
            "DisciplineGroupItem",
            "discipline",
            "disciplineId");
    qb.join(Sport::staticMetaObject, Sport::mapping(), "Discipline", "sport", "sportId");
    qb.where("DisciplineGroupItem", "disciplineGroupId", group->id());
    qb.orderBy("DisciplineGroupItem", "sort");

    QList<DisciplineGroupItem *> output = qb.query(db);

    foreach (DisciplineGroupItem *field, output) {
        field->setDisciplineGroup(group);
    }

    return output;
}
