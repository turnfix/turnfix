#include "entitymanager.h"
#include "repository/connectionrepository.h"
#include "repository/eventrepository.h"

EntityManager::EntityManager(QObject *parent)
    : QObject(parent)
{
    m_connectionRepository = new ConnectionRepository();
    m_eventRepository = new EventRepository();
}

EntityManager::~EntityManager()
{
    delete m_connectionRepository;
    delete m_eventRepository;
}

ConnectionRepository *EntityManager::connectionRepository() const
{
    return m_connectionRepository;
}

EventRepository *EntityManager::eventRepository() const
{
    return m_eventRepository;
}
