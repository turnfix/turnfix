#include "formularepository.h"
#include "model/entity/formula.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"

FormulaRepository::FormulaRepository(EntityManager *em)
    : AbstractRepository<Formula>(em)
{}

QList<Formula *> FormulaRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Formula> qb;
    qb.select(Formula::staticMetaObject, Formula::mapping());
    qb.orderBy("Formula", "name");

    QList<Formula *> output = qb.query(db);

    return output;
}
