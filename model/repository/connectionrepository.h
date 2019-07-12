#ifndef CONNECTIONREPOSITORY_H
#define CONNECTIONREPOSITORY_H

#include "abstractrepository.h"

class AbstractConnection;
class QSettings;

class ConnectionRepository : public AbstractRepository<AbstractConnection *>
{
public:
    explicit ConnectionRepository();

    QList<AbstractConnection *> loadAll() override;
    void persist(AbstractConnection *connection) override;
    void remove(AbstractConnection *connection) override;
};

#endif // CONNECTIONREPOSITORY_H
