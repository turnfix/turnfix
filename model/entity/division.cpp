#include "division.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Division::initializeMapping()
{
    DBTable *section = new DBTable("tfx_bereiche");
    section->addColumn("id", "int_bereicheid", ColumnType::Integer, 0, false, "", "", true);
    section->addColumn("name", "var_name", ColumnType::Varchar, 150, true);
    section->addColumn("male", "bol_maennlich", ColumnType::Boolean, 0, true, "'true'");
    section->addColumn("female", "bol_weiblich", ColumnType::Boolean, 0, true, "'true'");

    return section;
}

const DBTable *Division::m_mapping = Division::initializeMapping();

const DBTable *Division::mapping()
{
    return m_mapping;
}

int Division::id() const
{
    return m_id;
}

void Division::setId(int id)
{
    m_id = id;
}

QString Division::name() const
{
    return m_name;
}

void Division::setName(const QString &name)
{
    m_name = name;
}

bool Division::men() const
{
    return m_men;
}

void Division::setMen(bool men)
{
    m_men = men;
}

bool Division::women() const
{
    return m_women;
}

void Division::setWomen(bool women)
{
    m_women = women;
}
