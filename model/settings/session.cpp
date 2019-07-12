#include "session.h"
#include "model/entity/event.h"
#include <QMutex>

Session *Session::instance = nullptr;

void Session::setEvent(Event *event)
{
    this->event = event;
}

Event* Session::getEvent()
{
    return this->event;
}

Session* Session::getInstance()
{
    static QMutex mutex;
    if (!instance)
    {
        mutex.lock();

        if (!instance)
        {
            instance = new Session;
        }

        mutex.unlock();
    }
    return instance;
}

void Session::dropInstance()
{
    static QMutex mutex;
    mutex.lock();
    delete instance;
    instance = nullptr;
    mutex.unlock();
}
