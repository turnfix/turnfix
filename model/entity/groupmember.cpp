#include "groupmember.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *GroupeMember::initializeMapping()
{
    DBTable *groupeMember = new DBTable("tfx_gruppen_x_teilnehmer");
    groupeMember->addColumn("id", "int_gruppen_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    groupeMember->addColumn("groupeId", "int_gruppenid", ColumnType::Integer, 0, false)
            ->addContraint("fky_gruppenid", "tfx_gruppen", "int_gruppenid", "RESTRICT", "CASCADE");
    groupeMember->addColumn("athleteId", "int_teilnehmerid", ColumnType::Integer, 0, false)
            ->addContraint("fky_teilnehmerid", "tfx_teilnehmer", "int_teilnehmerid", "RESTRICT", "RESTRICT");

    return groupeMember;
}

const DBTable *GroupeMember::m_mapping = GroupeMember::initializeMapping();

const DBTable *GroupeMember::mapping()
{
    return m_mapping;
}

Groupe *GroupeMember::groupe() const
{
    return m_groupe;
}

void GroupeMember::setGroupe(Groupe *value)
{
    m_groupe = value;

    if (m_groupe == nullptr) {
        m_groupeId = 0;
        return;
    }

    m_groupeId = m_groupe->id();
}

int GroupeMember::groupeId() const
{
    return m_groupeId;
}

void GroupeMember::setGroupeId(int groupeId)
{
    m_groupeId = groupeId;
}

Athlete *GroupeMember::athlete() const
{
    return m_athlete;
}

void GroupeMember::setAthlete(Athlete *value)
{
    m_athlete = value;

    if (m_athlete == nullptr) {
        m_athleteId = 0;
        return;
    }

    m_athleteId = m_athlete->id();
}

int GroupeMember::athleteId() const
{
    return m_athleteId;
}

void GroupeMember::setAthleteId(int athleteId)
{
    m_athleteId = athleteId;
}
