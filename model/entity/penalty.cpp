#include "penalty.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Penalty::initializeMapping()
{
    DBTable *penalty = new DBTable("tfx_mannschaften_abzug");
    penalty->addColumn("id", "int_mannschaften_abzugid", ColumnType::Integer, 0, false, "", "", true);
    penalty->addColumn("name", "var_name", ColumnType::Varchar, 100);
    penalty->addColumn("deduction", "rel_abzug", ColumnType::Real);

    return penalty;
}

const DBTable *Penalty::m_mapping = Penalty::initializeMapping();

const DBTable *Penalty::mapping()
{
    return m_mapping;
}

int Penalty::id() const
{
    return m_id;
}

void Penalty::setId(int id)
{
    m_id = id;
}

QString Penalty::name() const
{
    return m_name;
}

void Penalty::setName(const QString &name)
{
    m_name = name;
}

double Penalty::deduction() const
{
    return m_deduction;
}

void Penalty::setDeduction(double deduction)
{
    m_deduction = deduction;
}
