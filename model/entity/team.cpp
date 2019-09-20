#include "team.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Team::initializeMapping()
{
    DBTable *team = new DBTable("tfx_mannschaften");
    team->addColumn("id", "int_mannschaftenid", ColumnType::Integer, 0, false, "", "", true);
    team->addColumn("competitionId", "int_wettkaempfeid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wettkaempfeid", "tfx_wettkaempfe", "int_vereineid", "RESTRICT", "RESTRICT");
    team->addColumn("clubId", "int_vereineid", ColumnType::Integer, 0, false)
        ->addContraint("fky_vereineid", "tfx_vereine", "int_vereineid", "RESTRICT", "RESTRICT");
    team->addColumn("number", "int_nummer", ColumnType::Integer, 0, false);
    team->addColumn("squad", "var_riege", ColumnType::Varchar, 150);
    team->addColumn("bib", "int_startnummer", ColumnType::Integer, 0, false);

    return team;
}

const DBTable *Team::m_mapping = Team::initializeMapping();

const DBTable *Team::mapping()
{
    return m_mapping;
}

Club *Team::club() const
{
    return m_club;
}

void Team::setClub(Club *value)
{
    m_club = value;

    if (m_club == nullptr) {
        m_clubId = 0;
        return;
    }

    m_clubId = m_club->id();
}

Competition *Team::competition() const
{
    return m_competition;
}

void Team::setCompoetition(Competition *value)
{
    m_competition = value;

    if(m_competition == nullptr){
        m_competitionId = 0;
        return;
    }

    m_competitionId = m_competition->id();
}

int Team::id() const
{
    return m_id;
}

void Team::setId(int id)
{
    m_id = id;
}

int Team::clubId() const
{
    return m_clubId;
}

void Team::setClubId(int clubId)
{
    m_clubId = clubId;
}

int Team::competitionId() const
{
    return  m_competitionId;
}

void Team::setCompetitionId(int competitionId)
{
    m_competitionId = competitionId;
}

QString Team::squad() const
{
    return m_squad;
}

void Team::setSquad(const QString &squad)
{
    m_squad = squad;
}

int Team::number() const
{
    return m_number;
}

void Team::setNumber(int number)
{
    m_number = number;
}

int Team::bib() const
{
    return m_bib;
}

void Team::setBib(int bib)
{
    m_bib = bib;
}

