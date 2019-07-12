#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QObject>

class ConnectionRepository;
class EventRepository;

class EntityManager : public QObject
{
    Q_OBJECT

public:
    EntityManager(QObject *parent = nullptr);
    ~EntityManager() override;

    ConnectionRepository *connectionRepository() const;
    EventRepository *eventRepository() const;

private:
    ConnectionRepository *m_connectionRepository;
    EventRepository *m_eventRepository;
};

#endif // ENTITYMANAGER_H
