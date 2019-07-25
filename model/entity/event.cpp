#include "event.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include <QVariant>

DBTable *Event::initializeMapping()
{
    DBTable *event = new DBTable("tfx_veranstaltungen");
    event->addColumn("id", "int_veranstaltungenid", ColumnType::Integer, 0, false, "", "", true);
    event->addColumn("venueId", "int_wettkampforteid", ColumnType::Integer, 0, false);
    event->addColumn("name", "var_name", ColumnType::Varchar, 250);
    event->addColumn("startDate", "dat_von", ColumnType::Date, 0, false);
    event->addColumn("endDate", "dat_bis", ColumnType::Date, 0, false);
    event->addColumn("multiRoundEvent",
                     "bol_rundenwettkampf",
                     ColumnType::Boolean,
                     0,
                     true,
                     "'false'");
    event->addColumn("round", "int_runde", ColumnType::SmallInt, 0, true, "1");
    event->addColumn("mainEventId", "int_hauptwettkampf", ColumnType::Integer);
    event->addColumn("registrationContactId", "int_meldung_an", ColumnType::Integer);
    event->addColumn("eventContactId", "int_ansprechpartner", ColumnType::Integer);
    event->addColumn("bankAccountId", "int_kontenid", ColumnType::Integer);
    event->addColumn("registrationDeadline", "dat_meldeschluss", ColumnType::Date);
    event->addColumn("organizer", "var_veranstalter", ColumnType::Varchar, 150);
    event->addColumn("itTeamCount", "int_edv", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("volunteersCount", "int_helfer", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("judgesCount", "int_kampfrichter", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("website", "var_meldung_website", ColumnType::Varchar, 200);
    event->addColumn("reference", "var_verwendungszweck", ColumnType::Varchar, 150);
    event->addColumn("registrationFee", "rel_meldegeld", ColumnType::Real, 0, true, "0");
    event->addColumn("lateFee", "rel_nachmeldung", ColumnType::Real, 0, true, "0");
    event->addColumn("chargeNoShow",
                     "bol_faellig_nichtantritt",
                     ColumnType::Boolean,
                     0,
                     true,
                     "'false'");
    event->addColumn("changesAllowed",
                     "bol_ummeldung_moeglich",
                     ColumnType::Boolean,
                     0,
                     true,
                     "'false'");
    event->addColumn("lateRegistrationAllowed",
                     "bol_nachmeldung_moeglich",
                     ColumnType::Boolean,
                     0,
                     true,
                     "'false'");
    event->addColumn("registrationNotes", "txt_meldung_an", ColumnType::Text);
    event->addColumn("licenseRequirements", "txt_startberechtigung", ColumnType::Text);
    event->addColumn("entryRequirements", "txt_teilnahmebedingungen", ColumnType::Text);
    event->addColumn("awards", "txt_siegerauszeichnung", ColumnType::Text);
    event->addColumn("judges", "txt_kampfrichter", ColumnType::Text);
    event->addColumn("notes", "txt_hinweise", ColumnType::Text);
    event->addContraint("fky_ansprechpartner",
                        "tfx_personen",
                        "int_ansprechpartner",
                        "int_personenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_hauptwettkampf",
                        "tfx_veranstaltungen",
                        "int_hauptwettkampf",
                        "int_veranstaltungenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_kontenid",
                        "tfx_konten",
                        "int_kontenid",
                        "int_kontenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_meldung_an",
                        "tfx_personen",
                        "int_meldung_an",
                        "int_personenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_wettkampforteid",
                        "tfx_wettkampforte",
                        "int_wettkampforteid",
                        "int_wettkampforteid",
                        "RESTRICT",
                        "RESTRICT");

    return event;
}

const DBTable *Event::m_mapping = Event::initializeMapping();

const DBTable *Event::mapping()
{
    return m_mapping;
}

void Event::setMainEvent(Event *mainEvent)
{
    m_mainEvent = mainEvent;
    if (mainEvent == nullptr) {
        m_mainEventId = 0;
        return;
    }

    m_mainEventId = mainEvent->id();
}

int Event::mainEventId() const
{
    return m_mainEventId;
}

int Event::venueId() const
{
    return m_venueId;
}

int Event::registrationContactId() const
{
    return m_registrationContactId;
}

int Event::bankAccountId() const
{
    return m_bankAccountId;
}

int Event::eventContactId() const
{
    return m_eventContactId;
}

void Event::setBankAccount(BankAccount *bankAccount)
{
    m_bankAccount = bankAccount;
    if (bankAccount == nullptr) {
        m_bankAccountId = 0;
        return;
    }

    m_bankAccountId = bankAccount->id();
}

void Event::setEventContact(Person *eventContact)
{
    m_eventContact = eventContact;
    if (eventContact == nullptr) {
        m_eventContactId = 0;
        return;
    }

    m_eventContactId = m_eventContact->id();
}

void Event::setRegistrationContact(Person *registrationContact)
{
    m_registrationContact = registrationContact;
    if (registrationContact == nullptr) {
        m_registrationContactId = 0;
        return;
    }

    m_registrationContactId = registrationContact->id();
}

void Event::setVenue(Venue *venue)
{
    m_venue = venue;
    if (venue == nullptr) {
        m_venueId = 0;
        return;
    }

    m_venueId = venue->id();
}

void Event::setId(int id)
{
    m_id = id;
}

QString Event::registrationNotes() const
{
    return m_registrationNotes;
}

void Event::setRegistrationNotes(const QString &registrationNotes)
{
    m_registrationNotes = registrationNotes;
}

QString Event::licenseRequirements() const
{
    return m_licenseRequirements;
}

void Event::setLicenseRequirements(const QString &licenseRequirements)
{
    m_licenseRequirements = licenseRequirements;
}

QString Event::organizer() const
{
    return m_organizer;
}

void Event::setOrganizer(const QString &organizer)
{
    m_organizer = organizer;
}

void Event::setRegistrationContactId(int registrationContactId)
{
    m_registrationContactId = registrationContactId;
}

Person *Event::registrationContact() const
{
    return m_registrationContact;
}

QString Event::website() const
{
    return m_website;
}

void Event::setWebsite(const QString &website)
{
    m_website = website;
}

double Event::registrationFee() const
{
    return m_registrationFee;
}

void Event::setRegistrationFee(double registrationFee)
{
    m_registrationFee = registrationFee;
}

bool Event::chargeNoShow() const
{
    return m_chargeNoShow;
}

void Event::setChargeNoShow(bool chargeNoShow)
{
    m_chargeNoShow = chargeNoShow;
}

bool Event::changesAllowed() const
{
    return m_changesAllowed;
}

void Event::setChangesAllowed(bool changesAllowed)
{
    m_changesAllowed = changesAllowed;
}

bool Event::lateRegistrationAllowed() const
{
    return m_lateRegistrationAllowed;
}

void Event::setLateRegistrationAllowed(bool lateRegistrationAllowed)
{
    m_lateRegistrationAllowed = lateRegistrationAllowed;
}

double Event::lateFee() const
{
    return m_lateFee;
}

void Event::setLateFee(double lateFee)
{
    m_lateFee = lateFee;
}

void Event::setBankAccountId(int bankAccoundId)
{
    m_bankAccountId = bankAccoundId;
}

BankAccount *Event::bankAccount() const
{
    return m_bankAccount;
}

QString Event::reference() const
{
    return m_reference;
}

void Event::setReference(const QString &reference)
{
    m_reference = reference;
}

QString Event::entryRequirements() const
{
    return m_entryRequirements;
}

void Event::setEntryRequirements(const QString &entryRequirements)
{
    m_entryRequirements = entryRequirements;
}

QString Event::awards() const
{
    return m_awards;
}

void Event::setAwards(const QString &awards)
{
    m_awards = awards;
}

QString Event::judges() const
{
    return m_judges;
}

void Event::setJudges(const QString &judges)
{
    m_judges = judges;
}

QString Event::notes() const
{
    return m_notes;
}

void Event::setNotes(const QString &notes)
{
    m_notes = notes;
}

int Event::itTeamCount() const
{
    return m_itTeamCount;
}

void Event::setItTeamCount(int itTeamCount)
{
    m_itTeamCount = itTeamCount;
}

int Event::volunteersCount() const
{
    return m_volunteersCount;
}

void Event::setVolunteersCount(int volunteersCount)
{
    m_volunteersCount = volunteersCount;
}

int Event::judgesCount() const
{
    return m_judgesCount;
}

void Event::setJudgesCount(int judgesCount)
{
    m_judgesCount = judgesCount;
}

void Event::setRegistrationDeadline(const QDate &registrationDeadline)
{
    m_registrationDeadline = registrationDeadline;
}

void Event::setEventContactId(int eventContactId)
{
    m_eventContactId = eventContactId;
}

void Event::setMainEventId(int mainEventId)
{
    m_mainEventId = mainEventId;
}

void Event::setVenueId(int locationId)
{
    m_venueId = locationId;
}

Person *Event::eventContact() const
{
    return m_eventContact;
}

void Event::setRound(int round)
{
    m_round = round;
}

void Event::setMultiRoundEvent(bool multiRoundEvent)
{
    m_multiRoundEvent = multiRoundEvent;
}

void Event::setEndDate(const QDate &endDate)
{
    m_endDate = endDate;
}

void Event::setStartDate(const QDate &startDate)
{
    m_startDate = startDate;
}

void Event::setName(const QString &name)
{
    m_name = name;
}

int Event::id()
{
    return m_id;
}

Venue *Event::venue()
{
    return m_venue;
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

Event *Event::mainEvent()
{
    if (m_mainEvent != nullptr) {
        return m_mainEvent;
    }

    return this;
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
