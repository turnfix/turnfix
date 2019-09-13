#include "juryresults.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Juryresults::initializeMapping ()
{
    DBTable *juryJudgement = new DBTable("tfx_jury_results");
    juryJudgement->addColumn("id", "int_juryresultsid", ColumnType::Integer, 0, false, "", "", true);
    juryJudgement->addColumn("ratingId", "int_wertungenid", ColumnType::Integer, 0, false)
            ->addContraint("fky_wertungenid", "tfx_wertungen", "int_wertungenid", "RESTRICT", "CASCADE");
    juryJudgement->addColumn("disciplineFieldId", "int_disziplinen_felderid", ColumnType::Integer, 0, false)
            ->addContraint("fky_disziplinen_felderid", "tfx_disziplinen_felder", "int_disziplinen_felderid", "RESTRICT", "RESTRICT");
    juryJudgement->addColumn("trialCount", "int_versuch", ColumnType::SmallInt);
    juryJudgement->addColumn("performance", "rel_leistung", ColumnType::Real);
    juryJudgement->addColumn("kp", "int_kp", ColumnType::SmallInt);

    return juryJudgement;
}

const DBTable *Juryresults::m_mapping = Juryresults::initializeMapping();

const DBTable *Juryresults::mapping()
{
    return m_mapping;
}

int Juryresults::id() const
{
    return m_id;
}

void Juryresults::setId(int id)
{
    m_id = id;
}

Rating *Juryresults::rating() const
{
    return m_rating;
}

void Juryresults::setRating(Rating *value)
{
    m_rating = value;
    if (m_rating == nullptr){
        m_ratingId = 0;
        return;
    }

    m_ratingId = m_rating->id();
}

int Juryresults::ratingId() const
{
    return m_ratingId;
}

DisciplineField *Juryresults::disciplineField() const
{
    return m_disciplineField;
}

void Juryresults::setParticipant(DisciplineField *value)
{
    m_disciplineField = value;
    if(m_disciplineField == nullptr){
        m_disciplineFieldId = 0;
        return;
    }

    m_disciplineFieldId = m_disciplineField->id();
}

int Juryresults::disciplineId() const
{
    return m_disciplineFieldId;
}

int Juryresults::round() const
{
    return m_round;
}

void Juryresults::setRound(int round)
{
    m_round = round;
}

int Juryresults::trialCount() const
{
    return m_trialCount;
}

void Juryresults::settrialCount(int count)
{
    m_trialCount = count;
}

double Juryresults::performance() const
{
    return m_performance;
}

void Juryresults::setPerformance(double performance)
{
    m_performance = performance;
}

