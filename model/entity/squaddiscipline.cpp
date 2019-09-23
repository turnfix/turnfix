#include "squaddiscipline.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *SquadDiscipline::initializeMapping()
{
    DBTable *squadDiscipline = new DBTable("tfx_riegen_x_disziplinen");
    squadDiscipline
        ->addColumn("id", "int_riegen_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    squadDiscipline->addColumn("eventId", "int_veranstaltungenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_veranstaltungenid",
                       "tfx_veranstaltungen",
                       "int_veranstaltungenid",
                       "RESTRICT",
                       "CASCADE");
    squadDiscipline->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinenid",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "CASCADE");
    squadDiscipline->addColumn("statusId", "int_statusid", ColumnType::Integer, 0, false)
        ->addContraint("fky_statusid", "tfx_status", "int_statusid", "RESTRICT", "RESTRICT");
    squadDiscipline->addColumn("squad", "var_riege", ColumnType::Varchar, 5);
    squadDiscipline->addColumn("round", "int_runde", ColumnType::SmallInt);
    squadDiscipline->addColumn("start", "bol_erstes_geraet", ColumnType::Boolean, 0, true, "'false'");

    return squadDiscipline;
}

const DBTable *SquadDiscipline::m_mapping = SquadDiscipline::initializeMapping();

const DBTable *SquadDiscipline::mapping()
{
    return m_mapping;
}

int SquadDiscipline::id() const
{
    return m_id;
}

void SquadDiscipline::setId(int id)
{
    m_id = id;
}

int SquadDiscipline::eventId() const
{
    return m_eventId;
}

void SquadDiscipline::setEventId(int eventId)
{
    m_eventId = eventId;
}

Event *SquadDiscipline::event() const
{
    return m_event;
}

void SquadDiscipline::setEvent(Event *event)
{
    m_event = event;
    if (event == nullptr) {
        m_eventId = 0;
        return;
    }

    m_eventId = event->id();
}

int SquadDiscipline::disciplineId() const
{
    return m_disciplineId;
}

void SquadDiscipline::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

Discipline *SquadDiscipline::discipline() const
{
    return m_discipline;
}

void SquadDiscipline::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}

int SquadDiscipline::statusId() const
{
    return m_statusId;
}

void SquadDiscipline::setStatusId(int statusId)
{
    m_statusId = statusId;
}

Status *SquadDiscipline::status() const
{
    return m_status;
}

void SquadDiscipline::setStatus(Status *status)
{
    m_status = status;
    if (status == nullptr) {
        m_statusId = 0;
        return;
    }

    m_statusId = status->id();
}

QString SquadDiscipline::squad() const
{
    return m_squad;
}

void SquadDiscipline::setSquad(const QString &squad)
{
    m_squad = squad;
}

int SquadDiscipline::round() const
{
    return m_round;
}

void SquadDiscipline::setRound(int round)
{
    m_round = round;
}

bool SquadDiscipline::start() const
{
    return m_start;
}

void SquadDiscipline::setStart(bool start)
{
    m_start = start;
}
