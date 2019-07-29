#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QObject>

class BankAccountRepository;
class ClubRepository;
class CompetitionRepository;
class ConnectionRepository;
class CountryRepository;
class DisciplineRepository;
class DisciplineFieldRepository;
class DivisionRepository;
class EventRepository;
class FormulaRepository;
class PersonRepository;
class SportRepository;
class StateRepository;
class VenueRepository;

class EntityManager : public QObject
{
    Q_OBJECT

public:
    EntityManager(QObject *parent = nullptr);

    BankAccountRepository *bankAccountRepository() const;
    ClubRepository *clubRepository() const;
    CompetitionRepository *competitionRepository() const;
    ConnectionRepository *connectionRepository() const;
    CountryRepository *countryRepository() const;
    DisciplineRepository *disciplineRepository() const;
    DisciplineFieldRepository *disciplineFieldRepository() const;
    DivisionRepository *divisionRepository() const;
    EventRepository *eventRepository() const;
    FormulaRepository *formulaRepository() const;
    PersonRepository *personRepository() const;
    SportRepository *sportRepository() const;
    StateRepository *stateRepository() const;
    VenueRepository *venueRepository() const;

    bool startTransaction();
    bool commitTransaction();

    QString connectionName() const;
    void setConnectionName(const QString &connectionName);

private:
    QString m_connectionName;
    BankAccountRepository *m_bankAccountRepository;
    ClubRepository *m_clubRepository;
    CompetitionRepository *m_competitionRepository;
    ConnectionRepository *m_connectionRepository;
    CountryRepository *m_countryRepository;
    DisciplineRepository *m_disciplineRepository;
    DisciplineFieldRepository *m_disciplineFieldRepository;
    DivisionRepository *m_divisionRepository;
    EventRepository *m_eventRepository;
    FormulaRepository *m_formulaRepository;
    PersonRepository *m_personRepository;
    SportRepository *m_sportRepository;
    StateRepository *m_stateRepository;
    VenueRepository *m_venueRepository;
};

#endif // ENTITYMANAGER_H
