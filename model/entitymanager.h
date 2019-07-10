#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "connectionrepository.h"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    ConnectionRepository *getConnectionRepository() const;

private:
    ConnectionRepository *connectionRepository;
};

#endif // ENTITYMANAGER_H
