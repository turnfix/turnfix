#include <QMutex>
#include "session.h"
#include "model/objects/event.h"

Session* Session::instance = 0;

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
    instance = 0;
    mutex.unlock();
}


