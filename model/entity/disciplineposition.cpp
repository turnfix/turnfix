#include "disciplineposition.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"
#include "model/entity/competitiondiscipline.h"

DBTable *DisciplinePosition::initializeMapping()
{
    DBTable *disciplinePosition = new DBTable("tfx_wettkaempfe_dispos");
    disciplinePosition
        ->addColumn("id", "int_wettkaempfe_disposid", ColumnType::Integer, 0, false, "", "", true);
    disciplinePosition
        ->addColumn("competitionDisciplineId",
                    "int_wettkaempfe_x_disziplinenid",
                    ColumnType::Integer,
                    0,
                    false)
        ->addContraint("fky_wettkaempfe_x_disziplinenid",
                       "tfx_wettkaempfe_x_disziplinen",
                       "int_wettkaempfe_x_disziplinenid",
                       "RESTRICT",
                       "RESTRICT");
    disciplinePosition->addColumn("x", "int_sortx", ColumnType::SmallInt, 0, true, "0");
    disciplinePosition->addColumn("y", "int_sorty", ColumnType::SmallInt, 0, true, "0");
    disciplinePosition->addColumn("type", "int_kp", ColumnType::SmallInt, 0, true, "0");

    return disciplinePosition;
}

const DBTable *DisciplinePosition::m_mapping = DisciplinePosition::initializeMapping();

const DBTable *DisciplinePosition::mapping()
{
    return m_mapping;
}

CompetitionDiscipline *DisciplinePosition::competitionDiscipline() const
{
    return m_competitionDiscipline;
}

void DisciplinePosition::setCompetitionDiscipline(CompetitionDiscipline *competitionDiscipline)
{
    m_competitionDiscipline = competitionDiscipline;
    if (competitionDiscipline == nullptr) {
        m_competitionDisciplineId = 0;
        return;
    }

    m_competitionDisciplineId = m_competitionDiscipline->id();
}

int DisciplinePosition::id() const
{
    return m_id;
}

void DisciplinePosition::setId(int id)
{
    m_id = id;
}

int DisciplinePosition::competitionDisciplineId() const
{
    return m_competitionDisciplineId;
}

void DisciplinePosition::setCompetitionDisciplineId(int competitionDisciplineId)
{
    m_competitionDisciplineId = competitionDisciplineId;
}

int DisciplinePosition::x() const
{
    return m_x;
}

void DisciplinePosition::setX(int x)
{
    m_x = x;
}

int DisciplinePosition::y() const
{
    return m_y;
}

void DisciplinePosition::setY(int y)
{
    m_y = y;
}

int DisciplinePosition::type() const
{
    return m_type;
}

void DisciplinePosition::setType(int type)
{
    m_type = type;
}
