#include "startingorder.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *StartingOrder::initializeMapping()
{
    DBTable *startingOrder = new DBTable("tfx_startreihenfolge");
    startingOrder
        ->addColumn("id", "int_startreihenfolgeid", ColumnType::Integer, 0, false, "", "", true);
    startingOrder->addColumn("scoreId", "int_wertungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wertungenid", "tfx_wertungen", "int_wertungenid", "RESTRICT", "RESTRICT");
    startingOrder->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinen",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "CASCADE");
    startingOrder->addColumn("position", "int_pos", ColumnType::SmallInt);
    startingOrder->addColumn("type", "int_kp", ColumnType::SmallInt, 0, true, "0");

    return startingOrder;
}

const DBTable *StartingOrder::m_mapping = StartingOrder::initializeMapping();

const DBTable *StartingOrder::mapping()
{
    return m_mapping;
}

int StartingOrder::id() const
{
    return m_id;
}

void StartingOrder::setId(int id)
{
    m_id = id;
}

int StartingOrder::scoreId() const
{
    return m_scoreId;
}

void StartingOrder::setScoreId(int scoreId)
{
    m_scoreId = scoreId;
}

Score *StartingOrder::score() const
{
    return m_score;
}

void StartingOrder::setScore(Score *score)
{
    m_score = score;
    if (score == nullptr) {
        m_scoreId = 0;
        return;
    }

    m_scoreId = score->id();
}

int StartingOrder::disciplineId() const
{
    return m_disciplineId;
}

void StartingOrder::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

int StartingOrder::position() const
{
    return m_position;
}

void StartingOrder::setPosition(int position)
{
    m_position = position;
}

int StartingOrder::type() const
{
    return m_type;
}

void StartingOrder::setType(int type)
{
    m_type = type;
}

Discipline *StartingOrder::discipline() const
{
    return m_discipline;
}

void StartingOrder::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}
