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
    void persist(AbstractConnection *connection) override;
    void remove(AbstractConnection *connection) override;
};

#endif // CONNECTIONREPOSITORY_H
