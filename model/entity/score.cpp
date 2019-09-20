#include "score.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Score::initializeMapping()
{
    DBTable *score = new DBTable("tfx_wertungen");
    score->addColumn("id", "int_wertungenid", ColumnType::Integer, 0, false, "", "", true);
    score->addColumn("competitionId", "int_wettkaempfeid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wettkaempfeid",
                       "tfx_wettkaempfe",
                       "int_wettkaempfeid",
                       "RESTRICT",
                       "RESTRICT");
    score->addColumn("athleteId", "int_teilnehmerid", ColumnType::Integer)
        ->addContraint("fky_teilnehmerid",
                       "tfx_teilnehmer",
                       "int_teilnehmerid",
                       "RESTRICT",
                       "RESTRICT");
    score->addColumn("groupId", "int_gruppenid", ColumnType::Integer)
        ->addContraint("fky_gruppenid", "tfx_gruppen", "int_gruppenid", "RESTRICT", "RESTRICT");
    score->addColumn("teamId", "int_mannschaftenid", ColumnType::Integer)
        ->addContraint("fky_mannschaftenid",
                       "tfx_mannschaften",
                       "int_mannschaftenid",
                       "RESTRICT",
                       "RESTRICT");
    score->addColumn("statusId", "int_statusid", ColumnType::Integer, 0, false)
        ->addContraint("fky_statusid", "tfx_status", "int_statusid", "RESTRICT", "RESTRICT");
    score->addColumn("round", "int_runde", ColumnType::SmallInt, 0, true, "1");
    score->addColumn("bib", "int_startnummer", ColumnType::Integer, 0, true);
    score->addColumn("nonCompetitive", "bol_ak", ColumnType::Boolean, 0, true, "false");
    score->addColumn("dns", "bol_startet_nicht", ColumnType::Boolean, 0, true, "false");
    score->addColumn("squad", "var_riege", ColumnType::Varchar, 10);
    score->addColumn("comment", "var_comment", ColumnType::Varchar, 150);

    return score;
}

const DBTable *Score::m_mapping = Score::initializeMapping();

const DBTable *Score::mapping()
{
    return m_mapping;
}

int Score::id() const
{
    return m_id;
}

void Score::setId(int id)
{
    m_id = id;
}

Competition *Score::competition() const
{
    return m_competition;
}

void Score::setCompetition(Competition *competition)
{
    m_competition = competition;
    if (competition == nullptr) {
        m_competitionId = 0;
        return;
    }

    m_competitionId = competition->id();
}

int Score::competitionId() const
{
    return m_competitionId;
}

void Score::setCompetitionId(int competitionId)
{
    m_competitionId = competitionId;
}

Athlete *Score::athlete() const
{
    return m_athlete;
}

void Score::setAthlete(Athlete *athlete)
{
    m_athlete = athlete;
    if (athlete == nullptr) {
        m_athleteId = 0;
        return;
    }

    m_athleteId = athlete->id();
}

int Score::athleteId() const
{
    return m_athleteId;
}

void Score::setAthleteId(int athleteId)
{
    m_athleteId = athleteId;
}

Group *Score::group() const
{
    return m_group;
}

void Score::setGroup(Group *group)
{
    m_group = group;
    if (group == nullptr) {
        m_groupId = 0;
        return;
    }

    m_groupId = group->id();
}

int Score::groupId() const
{
    return m_groupId;
}

void Score::setGroupId(int groupId)
{
    m_groupId = groupId;
}

Team *Score::team() const
{
    return m_team;
}

void Score::setTeam(Team *team)
{
    m_team = team;
    if (team == nullptr) {
        m_teamId = 0;
        return;
    }

    m_teamId = team->id();
}

int Score::teamId() const
{
    return m_teamId;
}

void Score::setTeamId(int teamId)
{
    m_teamId = teamId;
}

Status *Score::status() const
{
    return m_status;
}

void Score::setStatus(Status *status)
{
    m_status = status;
    if (status == nullptr) {
        m_statusId = 0;
        return;
    }

    m_statusId = status->id();
}

int Score::statusId() const
{
    return m_statusId;
}

void Score::setStatusId(int stateId)
{
    m_statusId = stateId;
}

int Score::round() const
{
    return m_round;
}

void Score::setRound(int round)
{
    m_round = round;
}

bool Score::nonCompetitive() const
{
    return m_nonCompetitive;
}

void Score::setNonCompetitive(bool nonCompetitive)
{
    m_nonCompetitive = nonCompetitive;
}

bool Score::dns() const
{
    return m_dns;
}

void Score::setDns(bool dns)
{
    m_dns = dns;
}

QString Score::squad() const
{
    return m_squad;
}

void Score::setSquad(const QString &squad)
{
    m_squad = squad;
}

QString Score::comment() const
{
    return m_comment;
}

void Score::setComment(const QString &comment)
{
    m_comment = comment;
}
