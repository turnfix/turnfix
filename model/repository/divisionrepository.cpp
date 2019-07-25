#include "divisionrepository.h"
#include "model/entity/division.h"
#include "model/entitymanager.h"
#include <QSqlQuery>

DivisionRepository::DivisionRepository(EntityManager *em)
    : AbstractRepository<Division>(em)
{}

void DivisionRepository::persist(Division *division) {}

void DivisionRepository::remove(Division *division) {}
