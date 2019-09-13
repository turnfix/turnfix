#include "groupe.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Groupe::initializeMapping()
{
    DBTable *groupe = new DBTable("tfx_gruppen");
    groupe->addColumn("id", "int_gruppenid", ColumnType::Integer, 0, false, "", "", true);
    groupe->addColumn("clubId", "int_vereineid", ColumnType::Integer, 0, false)
        ->addContraint("fky_vereineid", "tfx_vereine", "int_vereineid", "RESTRICT", "RESTRICT");
    groupe->addColumn("name", "var_name", ColumnType::Varchar, 150);

    return groupe;
}

const DBTable *Groupe::m_mapping = Groupe::initializeMapping();

const DBTable *Groupe::mapping()
{
    return m_mapping;
}

Club *Groupe::club() const
{
    return m_club;
}

void Groupe::setClub(Club *value)
{
    m_club = value;

    if (m_club == nullptr) {
        m_clubId = 0;
        return;
    }

    m_clubId = m_club->id();
}

int Groupe::id() const
{
    return m_id;
}

void Groupe::setId(int id)
{
    m_id = id;
}

int Groupe::clubId() const
{
    return m_clubId;
}

void Groupe::setClubId(int clubId)
{
    m_clubId = clubId;
}

QString Groupe::name() const
{
    return m_name;
}

void Groupe::setName(const QString &name)
{
    m_name = name;
}
