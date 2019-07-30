#include "staterepository.h"
#include "model/entity/country.h"

StateRepository::StateRepository(EntityManager *em)
    : AbstractRepository<State>(em)
{}

QList<State *> StateRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<State> qb;
    qb.select(State::staticMetaObject, State::mapping());
    qb.join(Country::staticMetaObject, Country::mapping(), "State", "country", "countryId");
    qb.orderBy("State", "name");

    QList<State *> output = qb.query(db);

    return output;
}
