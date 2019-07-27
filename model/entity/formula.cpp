#include "formula.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Formula::initializeMapping()
{
    DBTable *formula = new DBTable("tfx_formeln");
    formula->addColumn("id", "int_formelid", ColumnType::Integer, 0, false, "", "", true);
    formula->addColumn("name", "var_name", ColumnType::Varchar, 100);
    formula->addColumn("formula", "var_formel", ColumnType::Varchar, 200);
    formula->addColumn("type", "int_typ", ColumnType::SmallInt, 0, true, "0");

    return formula;
}

const DBTable *Formula::m_mapping = Formula::initializeMapping();

const DBTable *Formula::mapping()
{
    return m_mapping;
}

int Formula::id() const
{
    return m_id;
}

void Formula::setId(int id)
{
    m_id = id;
}

QString Formula::name() const
{
    return m_name;
}

void Formula::setName(const QString &name)
{
    m_name = name;
}

QString Formula::formula() const
{
    return m_formula;
}

void Formula::setFormula(const QString &formula)
{
    m_formula = formula;
}

int Formula::type() const
{
    return m_type;
}

void Formula::setType(int type)
{
    m_type = type;
}
