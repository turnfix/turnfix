#include "layout.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Layout::initializeMapping()
{
    DBTable *layout = new DBTable("tfx_layouts");
    layout->addColumn("id", "int_layoutid", ColumnType::Integer, 0, false, "", "", true);
    layout->addColumn("name", "var_name", ColumnType::Varchar, 100);
    layout->addColumn("comment", "txt_comment", ColumnType::Text);

    return layout;
}

const DBTable *Layout::m_mapping = Layout::initializeMapping();

const DBTable *Layout::mapping()
{
    return m_mapping;
}

int Layout::id() const
{
    return m_id;
}

void Layout::setId(int id)
{
    m_id = id;
}

QString Layout::name() const
{
    return m_name;
}

void Layout::setName(const QString &name)
{
    m_name = name;
}

QString Layout::comment() const
{
    return m_comment;
}

void Layout::setComment(const QString &comment)
{
    m_comment = comment;
}
