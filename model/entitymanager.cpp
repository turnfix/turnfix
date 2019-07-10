#include "entitymanager.h"

EntityManager::EntityManager()
{
    connectionRepository = new ConnectionRepository();
}

EntityManager::~EntityManager()
{
    delete connectionRepository;
}

ConnectionRepository *EntityManager::getConnectionRepository() const
{
    return connectionRepository;
}
