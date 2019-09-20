#include "group.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Group::initializeMapping()
{
    DBTable *group = new DBTable("tfx_gruppen");
    group->addColumn("id", "int_gruppenid", ColumnType::Integer, 0, false, "", "", true);
    group->addColumn("clubId", "int_vereineid", ColumnType::Integer, 0, false)
        ->addContraint("fky_vereineid", "tfx_vereine", "int_vereineid", "RESTRICT", "RESTRICT");
    group->addColumn("name", "var_name", ColumnType::Varchar, 150);

    return group;
}

const DBTable *Group::m_mapping = Group::initializeMapping();

const DBTable *Group::mapping()
{
    return m_mapping;
}

Club *Group::club() const
{
    return m_club;
}

void Group::setClub(Club *value)
{
    m_club = value;

    if (m_club == nullptr) {
        m_clubId = 0;
        return;
    }

    m_clubId = m_club->id();
}

int Group::id() const
{
    return m_id;
}

void Group::setId(int id)
{
    m_id = id;
}

int Group::clubId() const
{
    return m_clubId;
}

void Group::setClubId(int clubId)
{
    m_clubId = clubId;
}

QString Group::name() const
{
    return m_name;
}

void Group::setName(const QString &name)
{
    m_name = name;
}
