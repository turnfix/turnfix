#ifndef PERSONREPOSITORY_H
#define PERSONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/person.h"

class PersonRepository : public AbstractRepository<Person>
{
public:
    explicit PersonRepository(EntityManager *em);

    QList<Person *> loadAll();
};

#endif // PERSONREPOSITORY_H
