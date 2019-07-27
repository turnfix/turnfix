#include "sport.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Sport::initializeMapping()
{
    DBTable *sport = new DBTable("tfx_sport");
    sport->addColumn("id", "int_sportid", ColumnType::Integer, 0, false, "", "", true);
    sport->addColumn("name", "var_name", ColumnType::Varchar, 100);

    return sport;
}

const DBTable *Sport::m_mapping = Sport::initializeMapping();

const DBTable *Sport::mapping()
{
    return m_mapping;
}

int Sport::id() const
{
    return m_id;
}

void Sport::setId(int id)
{
    m_id = id;
}

QString Sport::name() const
{
    return m_name;
}

void Sport::setName(const QString &name)
{
    m_name = name;
}
