#include "clubrepository.h"
#include "model/entity/person.h"
#include "model/entitymanager.h"
#include "model/querybuilder.h"
#include <QSqlQuery>

ClubRepository::ClubRepository(EntityManager *em)
    : AbstractRepository<Club>(em)
{}

QList<Club *> ClubRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Club> qb;
    qb.select(Club::staticMetaObject, Club::mapping());
    qb.join(Person::staticMetaObject, Person::mapping(), "Club", "contactPerson", "contactPersonId");
    qb.orderBy("Club", "name");

    QList<Club *> output = qb.query(db);

    return output;
}
