#ifndef PERSONREPOSITORY_H
#define PERSONREPOSITORY_H

#include "abstractrepository.h"

class Person;

class PersonRepository : public AbstractRepository<Person>
{
public:
    explicit PersonRepository(EntityManager *em);

    QList<Person *> loadAll();
    void persist(Person *person) override;
    void remove(Person *person) override;
};

#endif // PERSONREPOSITORY_H
