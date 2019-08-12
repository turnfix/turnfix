#include "entitymanager.h"
#include "repository/athleterepository.h"
#include "repository/bankaccountrepository.h"
#include "repository/clubrepository.h"
#include "repository/competitiondisciplinerepository.h"
#include "repository/competitionrepository.h"
#include "repository/connectionrepository.h"
#include "repository/countryrepository.h"
#include "repository/disciplinefieldrepository.h"
#include "repository/disciplinegroupitemrepository.h"
#include "repository/disciplinegrouprepository.h"
#include "repository/disciplinerepository.h"
#include "repository/divisionrepository.h"
#include "repository/eventrepository.h"
#include "repository/formularepository.h"
#include "repository/penaltyrepository.h"
#include "repository/personrepository.h"
#include "repository/regionrepository.h"
#include "repository/sportrepository.h"
#include "repository/staterepository.h"
#include "repository/statusrepository.h"
#include "repository/venuerepository.h"

EntityManager::EntityManager(QObject *parent)
    : QObject(parent)
{
    m_athleteRepository = new AthleteRepository(this);
    m_bankAccountRepository = new BankAccountRepository(this);
    m_clubRepository = new ClubRepository(this);
    m_competitionRepository = new CompetitionRepository(this);
    m_competitionDisciplineRepository = new CompetitionDisciplineRepository(this);
    m_connectionRepository = new ConnectionRepository(this);
    m_countryRepository = new CountryRepository(this);
    m_disciplineRepository = new DisciplineRepository(this);
    m_disciplineFieldRepository = new DisciplineFieldRepository(this);
    m_disciplineGroupRepository = new DisciplineGroupRepository(this);
    m_disciplineGroupItemRepository = new DisciplineGroupItemRepository(this);
    m_divisionRepository = new DivisionRepository(this);
    m_eventRepository = new EventRepository(this);
    m_formulaRepository = new FormulaRepository(this);
    m_penaltyRepository = new PenaltyRepository(this);
    m_personRepository = new PersonRepository(this);
    m_regionRepository = new RegionRepository(this);
    m_sportRepository = new SportRepository(this);
    m_stateRepository = new StateRepository(this);
    m_statusRepository = new StatusRepository(this);
    m_venueRepository = new VenueRepository(this);
}

BankAccountRepository *EntityManager::bankAccountRepository() const
{
    return m_bankAccountRepository;
}

ConnectionRepository *EntityManager::connectionRepository() const
{
    return m_connectionRepository;
}

EventRepository *EntityManager::eventRepository() const
{
    return m_eventRepository;
}

PersonRepository *EntityManager::personRepository() const
{
    return m_personRepository;
}

VenueRepository *EntityManager::venueRepository() const
{
    return m_venueRepository;
}

bool EntityManager::startTransaction()
{
    QSqlDatabase db = QSqlDatabase::database(connectionName());
    return db.transaction();
}

bool EntityManager::commitTransaction()
{
    QSqlDatabase db = QSqlDatabase::database(connectionName());
    return db.commit();
}

QString EntityManager::connectionName() const
{
    return m_connectionName;
}

void EntityManager::setConnectionName(const QString &connectionName)
{
    m_connectionName = connectionName;
}

CompetitionDisciplineRepository *EntityManager::competitionDisciplineRepository() const
{
    return m_competitionDisciplineRepository;
}

AthleteRepository *EntityManager::athleteRepository() const
{
    return m_athleteRepository;
}

DisciplineGroupItemRepository *EntityManager::disciplineGroupItemRepository() const
{
    return m_disciplineGroupItemRepository;
}

DisciplineGroupRepository *EntityManager::disciplineGroupRepository() const
{
    return m_disciplineGroupRepository;
}

PenaltyRepository *EntityManager::penaltyRepository() const
{
    return m_penaltyRepository;
}

RegionRepository *EntityManager::regionRepository() const
{
    return m_regionRepository;
}

StatusRepository *EntityManager::statusRepository() const
{
    return m_statusRepository;
}

StateRepository *EntityManager::stateRepository() const
{
    return m_stateRepository;
}

CountryRepository *EntityManager::countryRepository() const
{
    return m_countryRepository;
}

DisciplineRepository *EntityManager::disciplineRepository() const
{
    return m_disciplineRepository;
}

DisciplineFieldRepository *EntityManager::disciplineFieldRepository() const
{
    return m_disciplineFieldRepository;
}

FormulaRepository *EntityManager::formulaRepository() const
{
    return m_formulaRepository;
}

SportRepository *EntityManager::sportRepository() const
{
    return m_sportRepository;
}

DivisionRepository *EntityManager::divisionRepository() const
{
    return m_divisionRepository;
}

CompetitionRepository *EntityManager::competitionRepository() const
{
    return m_competitionRepository;
}

ClubRepository *EntityManager::clubRepository() const
{
    return m_clubRepository;
}
