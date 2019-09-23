#include "teammember.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *TeamMember::initializeMapping()
{
    DBTable *teamMember = new DBTable("tfx_man_x_teilnehmer");
    teamMember
        ->addColumn("id", "int_man_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    teamMember->addColumn("teamId", "int_mannschaftenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_mannschaftenid",
                       "tfx_mannschaften",
                       "int_mannschaftenid",
                       "RESTRICT",
                       "CASCADE");
    teamMember->addColumn("athleteId", "int_teilnehmerid", ColumnType::Integer, 0, false)
        ->addContraint("fky_teilnehmerid",
                       "tfx_teilnehmer",
                       "int_teilnehmerid",
                       "RESTRICT",
                       "RESTRICT");
    teamMember->addColumn("round", "int_runde", ColumnType::SmallInt);

    return teamMember;
}

const DBTable *TeamMember::m_mapping = TeamMember::initializeMapping();

const DBTable *TeamMember::mapping()
{
    return m_mapping;
}

int TeamMember::id() const
{
    return m_id;
}

void TeamMember::setId(int id)
{
    m_id = id;
}

int TeamMember::teamId() const
{
    return m_teamId;
}

void TeamMember::setTeamId(int teamId)
{
    m_teamId = teamId;
}

Team *TeamMember::team() const
{
    return m_team;
}

void TeamMember::setTeam(Team *team)
{
    m_team = team;
    if (team == nullptr) {
        m_teamId = 0;
        return;
    }

    m_teamId = team->id();
}

int TeamMember::athleteId() const
{
    return m_athleteId;
}

void TeamMember::setAthleteId(int athleteId)
{
    m_athleteId = athleteId;
}

Athlete *TeamMember::athlete() const
{
    return m_athlete;
}

void TeamMember::setAthlete(Athlete *athlete)
{
    m_athlete = athlete;
    if (athlete == nullptr) {
        m_athleteId = 0;
        return;
    }

    m_athleteId = athlete->id();
}

int TeamMember::round() const
{
    return m_round;
}

void TeamMember::setRound(int round)
{
    m_round = round;
}
