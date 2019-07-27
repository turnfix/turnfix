#include "divisionrepository.h"
#include "model/entitymanager.h"

DivisionRepository::DivisionRepository(EntityManager *em)
    : AbstractRepository<Division>(em)
{}
