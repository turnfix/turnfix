#ifndef COUNTRYREPOSITORY_H
#define COUNTRYREPOSITORY_H

#include "abstractrepository.h"

class Country;

class CountryRepository : public AbstractRepository<Country>
{
public:
    explicit CountryRepository(EntityManager *em);

    QList<Country *> loadAll();
};

#endif // COUNTRYREPOSITORY_H
