#include "disciplinegroupitem.h"
#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *DisciplineGroupItem::initializeMapping()
{
    DBTable *disciplinGroupItem = new DBTable("tfx_disgrp_x_disziplinen");
    disciplinGroupItem
        ->addColumn("id", "int_disgrp_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    disciplinGroupItem
        ->addColumn("disciplineGroupId", "int_disziplinen_gruppenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinen_gruppenid",
                       "tfx_disziplinen_gruppen",
                       "int_disziplinen_gruppenid",
                       "RESTRICT",
                       "RESTRICT");
    disciplinGroupItem
        ->addColumn("disciplineId", "int_disziplinenid", ColumnType::Integer, 0, false)
        ->addContraint("fky_disziplinen",
                       "tfx_disziplinen",
                       "int_disziplinenid",
                       "RESTRICT",
                       "CASCADE");
    disciplinGroupItem->addColumn("sort", "int_pos", ColumnType::SmallInt);

    return disciplinGroupItem;
}

const DBTable *DisciplineGroupItem::m_mapping = DisciplineGroupItem::initializeMapping();

const DBTable *DisciplineGroupItem::mapping()
{
    return m_mapping;
}

int DisciplineGroupItem::id() const
{
    return m_id;
}

void DisciplineGroupItem::setId(int id)
{
    m_id = id;
}

int DisciplineGroupItem::disciplineGroupId() const
{
    return m_disciplineGroupId;
}

void DisciplineGroupItem::setDisciplineGroupId(int disciplineGroupId)
{
    m_disciplineGroupId = disciplineGroupId;
}

int DisciplineGroupItem::disciplineId() const
{
    return m_disciplineId;
}

void DisciplineGroupItem::setDisciplineId(int disciplineId)
{
    m_disciplineId = disciplineId;
}

Discipline *DisciplineGroupItem::discipline() const
{
    return m_discipline;
}

void DisciplineGroupItem::setDiscipline(Discipline *discipline)
{
    m_discipline = discipline;
    if (m_discipline == nullptr) {
        m_disciplineId = 0;
        return;
    }

    m_disciplineId = discipline->id();
}

int DisciplineGroupItem::sort() const
{
    return m_sort;
}

void DisciplineGroupItem::setSort(int sort)
{
    m_sort = sort;
}

DisciplineGroup *DisciplineGroupItem::disciplineGroup() const
{
    return m_disciplineGroup;
}

void DisciplineGroupItem::setDisciplineGroup(DisciplineGroup *disciplineGroup)
{
    m_disciplineGroup = disciplineGroup;
    if (m_disciplineGroup == nullptr) {
        m_disciplineGroupId = 0;
        return;
    }

    m_disciplineGroupId = disciplineGroup->id();
}
