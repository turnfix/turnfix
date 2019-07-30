#include "state.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *State::initializeMapping()
{
    DBTable *state = new DBTable("tfx_verbaende");
    state->addColumn("id", "int_verbaendeid", ColumnType::Integer, 0, false, "", "", true);
    state->addColumn("countryId", "int_laenderid", ColumnType::Integer, 0, false)
        ->addContraint("fky_laenderid", "tfx_laender", "int_laenderid", "RESTRICT", "RESTRICT");
    state->addColumn("name", "var_name", ColumnType::Varchar, 150);
    state->addColumn("code", "var_kuerzel", ColumnType::Varchar, 8);

    return state;
}

const DBTable *State::m_mapping = State::initializeMapping();

const DBTable *State::mapping()
{
    return m_mapping;
}

int State::id() const
{
    return m_id;
}

void State::setId(int id)
{
    m_id = id;
}

int State::countryId() const
{
    return m_countryId;
}

void State::setCountryId(int countryId)
{
    m_countryId = countryId;
}

Country *State::country() const
{
    return m_country;
}

void State::setCountry(Country *country)
{
    m_country = country;
    if (country == nullptr) {
        m_countryId = 0;
        return;
    }

    m_countryId = country->id();
}

QString State::name() const
{
    return m_name;
}

void State::setName(const QString &name)
{
    m_name = name;
}

QString State::code() const
{
    return m_code;
}

void State::setCode(const QString &code)
{
    m_code = code;
}
