#include "teampenalty.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *TeamPenalty::initializeMapping()
{
    DBTable *teamPenalty = new DBTable("tfx_man_x_man_ab");
    teamPenalty->addColumn("id", "int_man_x_man_abid", ColumnType::Integer, 0, false, "", "", true);
    teamPenalty->addColumn("teamId", "int_mannschaftenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_mannschaften_abzugid",
                       "tfx_mannschaften_abzug",
                       "int_mannschaften_abzugid",
                       "RESTRICT",
                       "RESTRICT");
    teamPenalty->addColumn("penaltyId", "int_mannschaften_abzugid", ColumnType::Integer, 0, false)
        ->addContraint("fky_mannschaftenid",
                       "tfx_mannschaften",
                       "int_mannschaftenid",
                       "RESTRICT",
                       "CASCADE");

    return teamPenalty;
}

const DBTable *TeamPenalty::m_mapping = TeamPenalty::initializeMapping();

const DBTable *TeamPenalty::mapping()
{
    return m_mapping;
}

Team *TeamPenalty::team() const
{
    return m_team;
}

void TeamPenalty::setTeam(Team *team)
{
    m_team = team;
    if (team == nullptr) {
        m_teamId = 0;
        return;
    }

    m_teamId = team->id();
}

Penalty *TeamPenalty::penalty() const
{
    return m_penalty;
}

void TeamPenalty::setPenalty(Penalty *penalty)
{
    m_penalty = penalty;
    if (penalty == nullptr) {
        m_penaltyId = 0;
        return;
    }

    m_penaltyId = penalty->id();
}

int TeamPenalty::id() const
{
    return m_id;
}

void TeamPenalty::setId(int id)
{
    m_id = id;
}

int TeamPenalty::teamId() const
{
    return m_teamId;
}

void TeamPenalty::setTeamId(int teamId)
{
    m_teamId = teamId;
}

int TeamPenalty::penaltyId() const
{
    return m_penaltyId;
}

void TeamPenalty::setPenaltyId(int penaltyId)
{
    m_penaltyId = penaltyId;
}
