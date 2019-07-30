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
