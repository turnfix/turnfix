#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class Event;

class Session : public QObject
{
    Q_OBJECT
public:
    void setEvent(Event* event);
    Event* getEvent();

    static Session* getInstance();
    static void dropInstance();

private:
    Event* event;

    static Session* instance;

    Session() {}
    Session(const Session &);
    Session& operator=(const Session &);

};

#endif // SESSION_H
