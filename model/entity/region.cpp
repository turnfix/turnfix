#include "region.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Region::initializeMapping()
{
    DBTable *region = new DBTable("tfx_gaue");
    region->addColumn("id", "int_gaueid", ColumnType::Integer, 0, false, "", "", true);
    region->addColumn("stateId", "int_verbaendeid", ColumnType::Integer, 0, false)
        ->addContraint("fky_verbaendeid", "tfx_verbaende", "int_verbaendeid", "RESTRICT", "RESTRICT");
    region->addColumn("name", "var_name", ColumnType::Varchar, 150);
    region->addColumn("code", "var_kuerzel", ColumnType::Varchar, 15);

    return region;
}

const DBTable *Region::m_mapping = Region::initializeMapping();

const DBTable *Region::mapping()
{
    return m_mapping;
}

int Region::id() const
{
    return m_id;
}

void Region::setId(int id)
{
    m_id = id;
}

int Region::stateId() const
{
    return m_stateId;
}

void Region::setStateId(int stateId)
{
    m_stateId = stateId;
}

State *Region::state() const
{
    return m_state;
}

void Region::setState(State *state)
{
    m_state = state;
    if (state == nullptr) {
        m_stateId = 0;
        return;
    }

    m_stateId = state->id();
}

QString Region::name() const
{
    return m_name;
}

void Region::setName(const QString &name)
{
    m_name = name;
}

QString Region::code() const
{
    return m_code;
}

void Region::setCode(const QString &code)
{
    m_code = code;
}
