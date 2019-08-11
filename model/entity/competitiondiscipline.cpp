#include "competitiondiscipline.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *CompetitionDiscipline::initializeMapping()
{
    DBTable *competitionDiscipline = new DBTable("tfx_wettkaempfe_x_disziplinen");
    competitionDiscipline->addColumn("id",
                                     "int_wettkaempfe_x_disziplinenid",
                                     ColumnType::Integer,
                                     0,
                                     false,
                                     "",
                                     "",
                                     true);
    competitionDiscipline
        ->addColumn("competitionId", "int_wettkaempfeid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wettkaempfeid",
                       "tfx_wettkaempfe",
                       "int_wettkaempfeid",
                       "RESTRICT",
                       "CASCADE");
    competitionDiscipline
        ->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinenid",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "RESTRICT");
    competitionDiscipline->addColumn("invitationText",
                                     "var_ausschreibung",
                                     ColumnType::Varchar,
                                     100);
    competitionDiscipline->addColumn("sort", "int_sortierung", ColumnType::SmallInt);
    competitionDiscipline
        ->addColumn("freeAndCompulsary", "bol_kp", ColumnType::Boolean, 0, true, "'false'");
    competitionDiscipline->addColumn("maximumScore", "rel_max", ColumnType::Real, 0, true, "0");

    return competitionDiscipline;
}

const DBTable *CompetitionDiscipline::m_mapping = CompetitionDiscipline::initializeMapping();

const DBTable *CompetitionDiscipline::mapping()
{
    return m_mapping;
}

int CompetitionDiscipline::id() const
{
    return m_id;
}

void CompetitionDiscipline::setId(int id)
{
    m_id = id;
}

int CompetitionDiscipline::competitionId() const
{
    return m_competitionId;
}

void CompetitionDiscipline::setCompetitionId(int competitionId)
{
    m_competitionId = competitionId;
}

Competition *CompetitionDiscipline::competition() const
{
    return m_competition;
}

void CompetitionDiscipline::setCompetition(Competition *competition)
{
    m_competition = competition;
    if (competition == nullptr) {
        m_competitionId = 0;
        return;
    }

    m_competitionId = competition->id();
}

int CompetitionDiscipline::disciplineId() const
{
    return m_disciplineId;
}

void CompetitionDiscipline::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

Discipline *CompetitionDiscipline::discipline() const
{
    return m_discipline;
}

void CompetitionDiscipline::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}

QString CompetitionDiscipline::invitationText() const
{
    return m_invitationText;
}

void CompetitionDiscipline::setInvitationText(const QString &invitationText)
{
    m_invitationText = invitationText;
}

int CompetitionDiscipline::sort() const
{
    return m_sort;
}

void CompetitionDiscipline::setSort(int sort)
{
    m_sort = sort;
}

bool CompetitionDiscipline::freeAndCompulsary() const
{
    return m_freeAndCompulsary;
}

void CompetitionDiscipline::setFreeAndCompulsary(bool freeAndCompulsary)
{
    if (freeAndCompulsary != m_freeAndCompulsary) {
        m_freeAndCompulsary = freeAndCompulsary;
        emit freeAndCompulsaryChanged(freeAndCompulsary);
    }
}

double CompetitionDiscipline::maximumScore() const
{
    return m_maximumScore;
}

void CompetitionDiscipline::setMaximumScore(double maximumScore)
{
    m_maximumScore = maximumScore;
}
