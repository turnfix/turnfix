#ifndef CONNECTIONREPOSITORY_H
#define CONNECTIONREPOSITORY_H

#include "abstractrepository.h"
#include "model/entity/abstractconnection.h"

class QSettings;
class EntityManager;

class ConnectionRepository : public AbstractRepository<AbstractConnection>
{
public:
    explicit ConnectionRepository(EntityManager *em);

    QList<AbstractConnection *> loadAll();
    bool persist(AbstractConnection *connection) override;
    bool remove(AbstractConnection *connection) override;
};

#endif // CONNECTIONREPOSITORY_H
