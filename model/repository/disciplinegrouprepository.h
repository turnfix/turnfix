#ifndef DISCIPLINEGROUPREPOSITORY_H
#define DISCIPLINEGROUPREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/disciplinegroup.h"

class DisciplineGroupRepository : public AbstractRepository<DisciplineGroup>
{
public:
    explicit DisciplineGroupRepository(EntityManager *em);

    QList<DisciplineGroup *> loadAll();
};

#endif // DISCIPLINEGROUPREPOSITORY_H
