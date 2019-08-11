#include "disciplinegroup.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *DisciplineGroup::initializeMapping()
{
    DBTable *disciplinGroup = new DBTable("tfx_disziplinen_gruppen");
    disciplinGroup
        ->addColumn("id", "int_disziplinen_gruppenid", ColumnType::Integer, 0, false, "", "", true);
    disciplinGroup->addColumn("name", "var_name", ColumnType::Varchar, 100);
    disciplinGroup->addColumn("comment", "txt_comment", ColumnType::Text);

    return disciplinGroup;
}

const DBTable *DisciplineGroup::m_mapping = DisciplineGroup::initializeMapping();

const DBTable *DisciplineGroup::mapping()
{
    return m_mapping;
}

int DisciplineGroup::id() const
{
    return m_id;
}

void DisciplineGroup::setId(int id)
{
    if (id != m_id) {
        m_id = id;
        emit idChanged(id);
    }
}

QString DisciplineGroup::name() const
{
    return m_name;
}

void DisciplineGroup::setName(const QString &name)
{
    m_name = name;
}

QString DisciplineGroup::comment() const
{
    return m_comment;
}

void DisciplineGroup::setComment(const QString &comment)
{
    m_comment = comment;
}
