#include "disciplinefieldrepository.h"
#include "model/entity/discipline.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"

DisciplineFieldRepository::DisciplineFieldRepository(EntityManager *em)
    : AbstractRepository<DisciplineField>(em)
{

}

QList<DisciplineField *> DisciplineFieldRepository::loadByDiscipline(Discipline *discipline)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<DisciplineField> qb;
    qb.select(DisciplineField::staticMetaObject, DisciplineField::mapping());
    qb.where("DisciplineField", "disciplineId", discipline->id());
    qb.orderBy("DisciplineField", "sort");

    QList<DisciplineField *> output = qb.query(db);

    foreach (DisciplineField *field, output) {
        field->setDiscipline(discipline);
    }

    return output;
}
