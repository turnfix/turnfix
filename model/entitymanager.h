#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QObject>

class AthleteRepository;
class BankAccountRepository;
class ClubRepository;
class CompetitionRepository;
class ConnectionRepository;
class CountryRepository;
class DisciplineRepository;
class DisciplineFieldRepository;
class DisciplineGroupRepository;
class DisciplineGroupItemRepository;
class DivisionRepository;
class EventRepository;
class FormulaRepository;
class PenaltyRepository;
class PersonRepository;
class RegionRepository;
class SportRepository;
class StateRepository;
class StatusRepository;
class VenueRepository;

class EntityManager : public QObject
{
    Q_OBJECT

public:
    EntityManager(QObject *parent = nullptr);

    AthleteRepository *athleteRepository() const;
    BankAccountRepository *bankAccountRepository() const;
    ClubRepository *clubRepository() const;
    CompetitionRepository *competitionRepository() const;
    ConnectionRepository *connectionRepository() const;
    CountryRepository *countryRepository() const;
    DisciplineRepository *disciplineRepository() const;
    DisciplineFieldRepository *disciplineFieldRepository() const;
    DisciplineGroupRepository *disciplineGroupRepository() const;
    DisciplineGroupItemRepository *disciplineGroupItemRepository() const;
    DivisionRepository *divisionRepository() const;
    EventRepository *eventRepository() const;
    FormulaRepository *formulaRepository() const;
    PersonRepository *personRepository() const;
    PenaltyRepository *penaltyRepository() const;
    RegionRepository *regionRepository() const;
    SportRepository *sportRepository() const;
    StateRepository *stateRepository() const;
    StatusRepository *statusRepository() const;
    VenueRepository *venueRepository() const;

    bool startTransaction();
    bool commitTransaction();

    QString connectionName() const;
    void setConnectionName(const QString &connectionName);

private:
    QString m_connectionName;
    AthleteRepository *m_athleteRepository;
    BankAccountRepository *m_bankAccountRepository;
    ClubRepository *m_clubRepository;
    CompetitionRepository *m_competitionRepository;
    ConnectionRepository *m_connectionRepository;
    CountryRepository *m_countryRepository;
    DisciplineRepository *m_disciplineRepository;
    DisciplineFieldRepository *m_disciplineFieldRepository;
    DisciplineGroupRepository *m_disciplineGroupRepository;
    DisciplineGroupItemRepository *m_disciplineGroupItemRepository;
    DivisionRepository *m_divisionRepository;
    EventRepository *m_eventRepository;
    FormulaRepository *m_formulaRepository;
    PenaltyRepository *m_penaltyRepository;
    PersonRepository *m_personRepository;
    RegionRepository *m_regionRepository;
    SportRepository *m_sportRepository;
    StateRepository *m_stateRepository;
    StatusRepository *m_statusRepository;
    VenueRepository *m_venueRepository;
};

#endif // ENTITYMANAGER_H
