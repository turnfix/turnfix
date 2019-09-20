#include "juryscore.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *JuryScore::initializeMapping ()
{
    DBTable *juryJudgement = new DBTable("tfx_jury_results");
    juryJudgement->addColumn("id", "int_juryresultsid", ColumnType::Integer, 0, false, "", "", true);
    juryJudgement->addColumn("scoreId", "int_wertungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wertungenid", "tfx_wertungen", "int_wertungenid", "RESTRICT", "CASCADE");
    juryJudgement
        ->addColumn("disciplineFieldId", "int_disziplinen_felderid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinen_felderid",
                       "tfx_disziplinen_felder",
                       "int_disziplinen_felderid",
                       "RESTRICT",
                       "RESTRICT");
    juryJudgement->addColumn("attempt", "int_versuch", ColumnType::SmallInt);
    juryJudgement->addColumn("performance", "rel_leistung", ColumnType::Real);
    juryJudgement->addColumn("type", "int_kp", ColumnType::SmallInt);

    return juryJudgement;
}

const DBTable *JuryScore::m_mapping = JuryScore::initializeMapping();

const DBTable *JuryScore::mapping()
{
    return m_mapping;
}

int JuryScore::id() const
{
    return m_id;
}

void JuryScore::setId(int id)
{
    m_id = id;
}

Score *JuryScore::score() const
{
    return m_score;
}

void JuryScore::setScore(Score *value)
{
    m_score = value;
    if (m_score == nullptr){
        m_scoreId = 0;
        return;
    }

    m_scoreId = m_score->id();
}

int JuryScore::scoreId() const
{
    return m_scoreId;
}

DisciplineField *JuryScore::disciplineField() const
{
    return m_disciplineField;
}

void JuryScore::setParticipant(DisciplineField *value)
{
    m_disciplineField = value;
    if(m_disciplineField == nullptr){
        m_disciplineFieldId = 0;
        return;
    }

    m_disciplineFieldId = m_disciplineField->id();
}

int JuryScore::round() const
{
    return m_round;
}

void JuryScore::setRound(int round)
{
    m_round = round;
}

int JuryScore::attempt() const
{
    return m_attempt;
}

void JuryScore::setAttempt(int count)
{
    m_attempt = count;
}

double JuryScore::performance() const
{
    return m_performance;
}

void JuryScore::setPerformance(double performance)
{
    m_performance = performance;
}

int JuryScore::type() const
{
    return m_type;
}

void JuryScore::setType(int type)
{
    m_type = type;
}

void JuryScore::setScoreId(int scoreId)
{
    m_scoreId = scoreId;
}

int JuryScore::disciplineFieldId() const
{
    return m_disciplineFieldId;
}

void JuryScore::setDisciplineFieldId(int disciplineFieldId)
{
    m_disciplineFieldId = disciplineFieldId;
}

