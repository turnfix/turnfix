#include "clubrepository.h"
#include "model/entity/club.h"
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

void ClubRepository::persist(Club *club)
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());
    QSqlQuery query(db);
    if (club->id() == 0) {
        query.prepare(
            "INSERT INTO tfx_vereine "
            "(int_personenid,var_name,int_start_ort,var_website,int_gaueid) VALUES (?,?,?,?,?)");
    } else {
        query.prepare("UPDATE tfx_vereine SET "
                      "int_personenid=?,var_name=?,int_start_ort=?,var_website=?,int_gaueid=? "
                      "WHERE int_vereineid=?");
        query.bindValue(5, club->id());
    }
    query.bindValue(0, club->contactPersonId());
    query.bindValue(1, club->name());
    query.bindValue(2, club->posCity());
    query.bindValue(3, club->website());
    query.bindValue(4, club->regionId());
    query.exec();

    if (club->id() == 0) {
        club->setId(query.lastInsertId().toInt());
    }
}

void ClubRepository::remove(Club *club) {}
