#include "status.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Status::initializeMapping()
{
    DBTable *status = new DBTable("tfx_status");
    status->addColumn("id", "int_statusid", ColumnType::Integer, 0, false, "", "", true);
    status->addColumn("name", "var_name", ColumnType::Varchar, 150);
    status->addColumn("color", "ary_colorcode", ColumnType::Varchar, 25);
    status->addColumn("scoresheet", "bol_bogen", ColumnType::Boolean, 0, true, "'true'");
    status->addColumn("scorecard", "bol_karte", ColumnType::Boolean, 0, true, "'true'");

    return status;
}

const DBTable *Status::m_mapping = Status::initializeMapping();

const DBTable *Status::mapping()
{
    return m_mapping;
}

int Status::id() const
{
    return m_id;
}

void Status::setId(int id)
{
    m_id = id;
}

QString Status::name() const
{
    return m_name;
}

void Status::setName(const QString &name)
{
    m_name = name;
}

QString Status::color() const
{
    return m_color;
}

void Status::setColor(const QString &color)
{
    m_color = color;
}

bool Status::scoresheet() const
{
    return m_scoresheet;
}

void Status::setScoresheet(bool scoresheet)
{
    m_scoresheet = scoresheet;
}

bool Status::scorecard() const
{
    return m_scorecard;
}

void Status::setScorecard(bool scorecard)
{
    m_scorecard = scorecard;
}
