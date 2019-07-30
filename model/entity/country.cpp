#include "country.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Country::initializeMapping()
{
    DBTable *country = new DBTable("tfx_laender");
    country->addColumn("id", "int_laenderid", ColumnType::Integer, 0, false, "", "", true);
    country->addColumn("name", "var_name", ColumnType::Varchar, 150);
    country->addColumn("code", "var_kuerzel", ColumnType::Varchar, 4);

    return country;
}

const DBTable *Country::m_mapping = Country::initializeMapping();

const DBTable *Country::mapping()
{
    return m_mapping;
}

int Country::id() const
{
    return m_id;
}

void Country::setId(int id)
{
    m_id = id;
}

QString Country::name() const
{
    return m_name;
}

void Country::setName(const QString &name)
{
    m_name = name;
}

QString Country::code() const
{
    return m_code;
}

void Country::setCode(const QString &code)
{
    m_code = code;
}
