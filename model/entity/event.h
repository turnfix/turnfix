#ifndef EVENT_H
#define EVENT_H

#include "bankaccount.h"
#include "person.h"
#include "venue.h"
#include <QDate>
#include <QObject>

class DBTable;

class Event : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int venueId READ venueId WRITE setVenueId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate)
    Q_PROPERTY(bool multiRoundEvent READ multiRoundEvent WRITE setMultiRoundEvent)
    Q_PROPERTY(int round READ round WRITE setRound)
    Q_PROPERTY(int mainEventId READ mainEventId WRITE setMainEventId)
    Q_PROPERTY(int eventContactId READ eventContactId WRITE setEventContactId)
    Q_PROPERTY(QDate registrationDeadline READ registrationDeadline WRITE setRegistrationDeadline)
    Q_PROPERTY(QString organizer READ organizer WRITE setOrganizer)
    Q_PROPERTY(int registrationContactId READ registrationContactId WRITE setRegistrationContactId)
    Q_PROPERTY(QString website READ website WRITE setWebsite)
    Q_PROPERTY(double registrationFee READ registrationFee WRITE setRegistrationFee)
    Q_PROPERTY(bool chargeNoShow READ chargeNoShow WRITE setChargeNoShow)
    Q_PROPERTY(bool changesAllowed READ changesAllowed WRITE setChangesAllowed)
    Q_PROPERTY(
        bool lateRegistrationAllowed READ lateRegistrationAllowed WRITE setLateRegistrationAllowed)
    Q_PROPERTY(double lateFee READ lateFee WRITE setLateFee)
    Q_PROPERTY(int bankAccountId READ bankAccountId WRITE setBankAccountId)
    Q_PROPERTY(QString reference READ reference WRITE setReference)
    Q_PROPERTY(QString entryRequirements READ entryRequirements WRITE setEntryRequirements)
    Q_PROPERTY(QString awards READ awards WRITE setAwards)
    Q_PROPERTY(QString judges READ judges WRITE setJudges)
    Q_PROPERTY(QString notes READ notes WRITE setNotes)
    Q_PROPERTY(int itTeamCount READ itTeamCount WRITE setItTeamCount)
    Q_PROPERTY(int volunteersCount READ volunteersCount WRITE setVolunteersCount)
    Q_PROPERTY(int judgesCount READ judgesCount WRITE setJudgesCount)
    Q_PROPERTY(QString licenseRequirements READ licenseRequirements WRITE setLicenseRequirements)
    Q_PROPERTY(QString registrationNotes READ registrationNotes WRITE setRegistrationNotes)

    Q_PROPERTY(Venue *venue READ venue WRITE setVenue STORED false)
    Q_PROPERTY(Person *eventContact READ eventContact WRITE setEventContact STORED false)
    Q_PROPERTY(Person *registrationContact READ registrationContact WRITE setRegistrationContact
                   STORED false)
    Q_PROPERTY(BankAccount *bankAccount READ bankAccount WRITE setBankAccount STORED false)

public:
    Q_INVOKABLE Event() = default;

    int id();
    void setId(int id);

    Venue *venue();
    void setVenue(Venue *venue);

    int venueId() const;
    void setVenueId(int locationId);

    QString name();
    void setName(const QString &name);

    QDate startDate();
    void setStartDate(const QDate &startDate);

    QDate endDate() const;
    void setEndDate(const QDate &endDate);

    bool multiRoundEvent();
    void setMultiRoundEvent(bool multiRoundEvent);

    void setRound(int round);
    int round();

    Event *mainEvent();
    void setMainEvent(Event *mainEvent);

    int mainEventId() const;
    void setMainEventId(int mainEventId);

    Person *eventContact() const;
    void setEventContact(Person *eventContact);

    int eventContactId() const;
    void setEventContactId(int eventContactId);

    QDate registrationDeadline() const;
    void setRegistrationDeadline(const QDate &registrationDeadline);

    QString organizer() const;
    void setOrganizer(const QString &organizer);

    Person *registrationContact() const;
    void setRegistrationContact(Person *registrationContact);

    int registrationContactId() const;
    void setRegistrationContactId(int registrationContactId);

    QString website() const;
    void setWebsite(const QString &website);

    double registrationFee() const;
    void setRegistrationFee(double registrationFee);

    bool chargeNoShow() const;
    void setChargeNoShow(bool chargeNoShow);

    bool changesAllowed() const;
    void setChangesAllowed(bool changesAllowed);

    bool lateRegistrationAllowed() const;
    void setLateRegistrationAllowed(bool lateRegistrationAllowed);

    double lateFee() const;
    void setLateFee(double lateFee);

    BankAccount *bankAccount() const;
    void setBankAccount(BankAccount *bankAccount);

    int bankAccountId() const;
    void setBankAccountId(int bankAccoundId);

    QString reference() const;
    void setReference(const QString &reference);

    QString entryRequirements() const;
    void setEntryRequirements(const QString &entryRequirements);

    QString awards() const;
    void setAwards(const QString &awards);

    QString judges() const;
    void setJudges(const QString &judges);

    QString notes() const;
    void setNotes(const QString &notes);

    int itTeamCount() const;
    void setItTeamCount(int itTeamCount);

    int volunteersCount() const;
    void setVolunteersCount(int volunteersCount);

    int judgesCount() const;
    void setJudgesCount(int judgesCount);

    QString licenseRequirements() const;
    void setLicenseRequirements(const QString &licenseRequirements);

    QString registrationNotes() const;
    void setRegistrationNotes(const QString &registrationNotes);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_venueId;
    Venue *m_venue;
    QString m_name;
    QDate m_startDate;
    QDate m_endDate;
    bool m_multiRoundEvent;
    int m_round;
    int m_mainEventId;
    Event *m_mainEvent;
    int m_eventContactId;
    Person *m_eventContact;
    QString m_organizer;

    int m_registrationContactId;
    Person *m_registrationContact;
    QDate m_registrationDeadline;
    QString m_website;
    double m_registrationFee;
    bool m_chargeNoShow;
    bool m_changesAllowed;
    bool m_lateRegistrationAllowed;
    double m_lateFee;
    int m_bankAccountId;
    BankAccount *m_bankAccount;
    QString m_reference;
    QString m_registrationNotes;
    QString m_licenseRequirements;
    QString m_entryRequirements;
    QString m_awards;
    QString m_judges;
    QString m_notes;

    int m_itTeamCount;
    int m_volunteersCount;
    int m_judgesCount;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Event *);

#endif // EVENT_H
