#include "scorediscipline.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *ScoreDiscipline::initializeMapping()
{
    DBTable *scoreDiscipline = new DBTable("tfx_wertungen_x_disziplinen");
    scoreDiscipline->addColumn("id",
                               "int_wertungen_x_disziplinenid",
                               ColumnType::Integer,
                               0,
                               false,
                               "",
                               "",
                               true);
    scoreDiscipline->addColumn("scoreId", "int_wertungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wertungenid", "tfx_wertungen", "int_wertungenid", "RESTRICT", "CASCADE");
    scoreDiscipline->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinenid",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "RESTRICT");

    return scoreDiscipline;
}

const DBTable *ScoreDiscipline::m_mapping = ScoreDiscipline::initializeMapping();

const DBTable *ScoreDiscipline::mapping()
{
    return m_mapping;
}

int ScoreDiscipline::id() const
{
    return m_id;
}

void ScoreDiscipline::setId(int id)
{
    m_id = id;
}

int ScoreDiscipline::scoreId() const
{
    return m_scoreId;
}

void ScoreDiscipline::setScoreId(int scoreId)
{
    m_scoreId = scoreId;
}

int ScoreDiscipline::disciplineId() const
{
    return m_disciplineId;
}

void ScoreDiscipline::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

Score *ScoreDiscipline::score() const
{
    return m_score;
}

void ScoreDiscipline::setScore(Score *score)
{
    m_score = score;
    if (score == nullptr) {
        m_scoreId = 0;
        return;
    }

    m_scoreId = m_score->id();
}

Discipline *ScoreDiscipline::discipline() const
{
    return m_discipline;
}

void ScoreDiscipline::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}
