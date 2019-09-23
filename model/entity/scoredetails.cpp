#include "scoredetails.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *ScoreDetails::initializeMapping()
{
    DBTable *scoreDetails = new DBTable("tfx_wertungen_details");
    scoreDetails
        ->addColumn("id", "int_wertungen_detailsid", ColumnType::Integer, 0, false, "", "", true);
    scoreDetails->addColumn("scoreId", "int_wertungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wertungenid", "tfx_wertungen", "int_wertungenid", "RESTRICT", "CASCADE");
    scoreDetails->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinenid",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "RESTRICT");
    scoreDetails->addColumn("attempt", "int_versuch", ColumnType::SmallInt);
    scoreDetails->addColumn("performance", "rel_leistung", ColumnType::Real);
    scoreDetails->addColumn("type", "int_kp", ColumnType::SmallInt, 0, true, "0");

    return scoreDetails;
}

const DBTable *ScoreDetails::m_mapping = ScoreDetails::initializeMapping();

const DBTable *ScoreDetails::mapping()
{
    return m_mapping;
}

int ScoreDetails::id() const
{
    return m_id;
}

void ScoreDetails::setId(int id)
{
    m_id = id;
}

int ScoreDetails::scoreId() const
{
    return m_scoreId;
}

void ScoreDetails::setScoreId(int scoreId)
{
    m_scoreId = scoreId;
}

Score *ScoreDetails::score() const
{
    return m_score;
}

void ScoreDetails::setScore(Score *score)
{
    m_score = score;
    if (score == nullptr) {
        m_scoreId = 0;
        return;
    }

    m_scoreId = score->id();
}

int ScoreDetails::disciplineId() const
{
    return m_disciplineId;
}

void ScoreDetails::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

Discipline *ScoreDetails::discipline() const
{
    return m_discipline;
}

void ScoreDetails::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}

int ScoreDetails::attempt() const
{
    return m_attempt;
}

void ScoreDetails::setAttempt(int attempt)
{
    m_attempt = attempt;
}

double ScoreDetails::performance() const
{
    return m_performance;
}

void ScoreDetails::setPerformance(double performance)
{
    m_performance = performance;
}

int ScoreDetails::type() const
{
    return m_type;
}

void ScoreDetails::setType(int type)
{
    m_type = type;
}
