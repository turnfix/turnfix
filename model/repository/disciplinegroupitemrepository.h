#ifndef DISCIPLINEGROUPITEMREPOSITORY_H
#define DISCIPLINEGROUPITEMREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/disciplinegroupitem.h"

class DisciplineGroup;

class DisciplineGroupItemRepository : public AbstractRepository<DisciplineGroupItem>
{
public:
    explicit DisciplineGroupItemRepository(EntityManager *em);

    QList<DisciplineGroupItem *> loadByGroup(DisciplineGroup *group);
};

#endif // DISCIPLINEGROUPITEMREPOSITORY_H
