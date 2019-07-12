#ifndef EVENT_H
#define EVENT_H

#include <QDate>
#include <QObject>

class Location;
class Person;

class Event : public QObject
{
    Q_OBJECT

public:
    int id();
    Location *location();
    QString name();
    QDate startDate();
    int mainEventId();
    int round();
    bool multiRoundEvent();

    QDate endDate() const;

    QDate registrationDeadline() const;

private:
    int m_id;
    Location *m_location;
    QString m_name;
    QDate m_startDate;
    QDate m_endDate;
    bool m_multiRoundEvent;
    int m_round;
    int m_mainEventId;
    int m_eventContactId;
    QString m_organizer;

    int m_registrationContactId;
    QDate m_registrationDeadline;
    QString m_website;
    QString m_registrationTo;
    double m_registrationFee;
    bool m_chargeNoShow;
    bool m_changesAllowed;
    bool m_lateRegistrationAllowed;
    double m_lateFee;
    int m_bankAccoundId;
    QString m_reference;
    QString m_entryRequirements;
    QString m_awards;
    QString m_judges;
    QString m_notes;

    int m_itTeamCount;
    int m_volunteersCount;
    int m_judgesCount;
};

#endif // EVENT_H
