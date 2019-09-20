#include "groupmember.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *GroupMember::initializeMapping()
{
    DBTable *groupMember = new DBTable("tfx_gruppen_x_teilnehmer");
    groupMember
        ->addColumn("id", "int_gruppen_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    groupMember->addColumn("groupId", "int_gruppenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_gruppenid", "tfx_gruppen", "int_gruppenid", "RESTRICT", "CASCADE");
    groupMember->addColumn("athleteId", "int_teilnehmerid", ColumnType::Integer, 0, false)
        ->addContraint("fky_teilnehmerid",
                       "tfx_teilnehmer",
                       "int_teilnehmerid",
                       "RESTRICT",
                       "RESTRICT");

    return groupMember;
}

const DBTable *GroupMember::m_mapping = GroupMember::initializeMapping();

const DBTable *GroupMember::mapping()
{
    return m_mapping;
}

int GroupMember::id() const
{
    return m_id;
}

void GroupMember::setId(int id)
{
    m_id = id;
}

Group *GroupMember::group() const
{
    return m_group;
}

void GroupMember::setGroup(Group *group)
{
    m_group = group;

    if (m_group == nullptr) {
        m_groupeId = 0;
        return;
    }

    m_groupeId = m_group->id();
}

int GroupMember::groupId() const
{
    return m_groupeId;
}

void GroupMember::setGroupId(int groupeId)
{
    m_groupeId = groupeId;
}

Athlete *GroupMember::athlete() const
{
    return m_athlete;
}

void GroupMember::setAthlete(Athlete *value)
{
    m_athlete = value;

    if (m_athlete == nullptr) {
        m_athleteId = 0;
        return;
    }

    m_athleteId = m_athlete->id();
}

int GroupMember::athleteId() const
{
    return m_athleteId;
}

void GroupMember::setAthleteId(int athleteId)
{
    m_athleteId = athleteId;
}
