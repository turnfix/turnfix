#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class Event;

class Session : public QObject
{
    Q_OBJECT
public:
    void setEvent(Event* m_event);
    Event* getEvent();

    static Session* getInstance();
    static void dropInstance();

private:
    Event* m_event;

    static Session* instance;

    Session() {}
    Session(const Session &);
    Session& operator=(const Session &);

};

#endif // SESSION_H
