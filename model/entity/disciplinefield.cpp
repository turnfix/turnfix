#include "disciplinefield.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *DisciplineField::initializeMapping()
{
    DBTable *disciplineField = new DBTable("tfx_disziplinen_felder");
    disciplineField
        ->addColumn("id", "int_disziplinen_felderid", ColumnType::Integer, 0, false, "", "", true);
    disciplineField->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false);
    disciplineField->addColumn("name", "var_name", ColumnType::Varchar, 15);
    disciplineField->addColumn("sort", "int_sortierung", ColumnType::SmallInt);
    disciplineField->addColumn("finalScore", "bol_endwert", ColumnType::Boolean, 0, true, "'true'");
    disciplineField
        ->addColumn("baseScore", "bol_ausgangswert", ColumnType::Boolean, 0, true, "'true'");
    disciplineField->addColumn("group", "int_gruppe", ColumnType::SmallInt, 0, true, "1");
    disciplineField->addColumn("enabled", "bol_enabled", ColumnType::Boolean, 0, true, "'true'");
    disciplineField->addContraint("fky_disziplinenid",
                                  "tfx_disziplinen",
                                  "int_disziplinenid",
                                  "int_disziplinenid",
                                  "RESTRICT",
                                  "CASCADE");

    return disciplineField;
}

const DBTable *DisciplineField::m_mapping = DisciplineField::initializeMapping();

const DBTable *DisciplineField::mapping()
{
    return m_mapping;
}

int DisciplineField::id() const
{
    return m_id;
}

void DisciplineField::setId(int id)
{
    m_id = id;
}

QString DisciplineField::name() const
{
    return m_name;
}

void DisciplineField::setName(const QString &name)
{
    m_name = name;
}

int DisciplineField::sort() const
{
    return m_sort;
}

void DisciplineField::setSort(int sort)
{
    m_sort = sort;
}

bool DisciplineField::finalScore() const
{
    return m_finalScore;
}

void DisciplineField::setFinalScore(bool finalScore)
{
    m_finalScore = finalScore;
}

bool DisciplineField::baseScore() const
{
    return m_baseScore;
}

void DisciplineField::setBaseScore(bool baseScore)
{
    m_baseScore = baseScore;
}

int DisciplineField::group() const
{
    return m_group;
}

void DisciplineField::setGroup(int group)
{
    m_group = group;
}

bool DisciplineField::enabled() const
{
    return m_enabled;
}

void DisciplineField::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
