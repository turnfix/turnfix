#include <QVariant>
#include "event.h"
#include "location.h"

int Event::id()
{
    return m_id;
}

Location *Event::location()
{
    return m_location;
}

QString Event::name()
{
    return m_name;
}

QDate Event::startDate()
{
    return m_startDate;
}

QDate Event::endDate() const
{
    return m_endDate;
}

int Event::mainEventId()
{
    if (m_mainEventId != 0) {
        return m_mainEventId;
    } else {
        return m_id;
    }
}

int Event::round()
{
    return m_round;
}

bool Event::multiRoundEvent()
{
    return m_multiRoundEvent;
}


QDate Event::registrationDeadline() const
{
    return m_registrationDeadline;
}
