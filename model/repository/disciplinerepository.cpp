#include "disciplinerepository.h"
#include "model/entity/formula.h"
#include "model/entity/sport.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"

DisciplineRepository::DisciplineRepository(EntityManager *em)
    : AbstractRepository<Discipline>(em)
{}

QList<Discipline *> DisciplineRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Discipline> qb;
    qb.select(Discipline::staticMetaObject, Discipline::mapping());
    qb.join(Sport::staticMetaObject, Sport::mapping(), "Discipline", "sport", "sportId");
    qb.join(Formula::staticMetaObject, Formula::mapping(), "Discipline", "formula", "formulaId");
    qb.orderBy("Discipline", "name");

    QList<Discipline *> output = qb.query(db);

    return output;
}

QList<Discipline *> DisciplineRepository::loadByGender(bool women, bool men)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Discipline> qb;
    qb.select(Discipline::staticMetaObject, Discipline::mapping());
    qb.join(Sport::staticMetaObject, Sport::mapping(), "Discipline", "sport", "sportId");
    qb.where("Discipline", "women", women);
    qb.where("Discipline", "men", men);
    qb.orderBy("Discipline", "name");

    QList<Discipline *> output = qb.query(db);

    return output;
}
