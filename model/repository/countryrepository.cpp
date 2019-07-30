#include "countryrepository.h"
#include "model/entity/country.h"

CountryRepository::CountryRepository(EntityManager *em)
    : AbstractRepository<Country>(em)
{}

QList<Country *> CountryRepository::loadAll()
{
    QSqlDatabase db = QSqlDatabase::database(entityManager()->connectionName());

    QueryBuilder<Country> qb;
    qb.select(Country::staticMetaObject, Country::mapping());
    qb.orderBy("Country", "name");

    QList<Country *> output = qb.query(db);

    return output;
}
