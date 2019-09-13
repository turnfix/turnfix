#include "rating.h"

#include "model/dbcolumn.h"
#include "model/dbtable.h"

DBTable *Rating::initializeMapping ()
{
    DBTable *rating = new DBTable("tfx_wertungen");
    rating->addColumn("id", "int_wertungenid", ColumnType::Integer, 0, false, "", "", true);
    rating->addColumn("competitionId", "int_wettkaempfeid", ColumnType::Integer, 0, false)
        ->addContraint("fky_wettkaempfeid", "tfx_wettkaempfe", "int_wettkaempfeid", "RESTRICT", "RESTRICT");
    rating->addColumn("participantId", "int_teilnehmerid", ColumnType::Integer)
        ->addContraint("fky_teilnehmerid", "tfx_teilnehmer", "int_teilnehmerid", "RESTRICT", "RESTRICT");
    rating->addColumn("groupeId", "int_gruppenid", ColumnType::Integer)
        ->addContraint("fky_gruppenid", "tfx_gruppen", "int_gruppenid", "RESTRICT", "RESTRICT");
    rating->addColumn("teamId", "int_mannschaftenid", ColumnType::Integer)
        ->addContraint("fky_mannschaftenid", "tfx_mannschaften", "int_mannschaftenid", "RESTRICT", "RESTRICT");
    rating->addColumn("stateId", "int_statusid", ColumnType::Integer, 0, false)
        ->addContraint("fky_statusid", "tfx_status", "int_statusid", "RESTRICT", "RESTRICT");
    rating->addColumn("round", "int_runde", ColumnType::SmallInt, 0, true, "1");
    rating->addColumn("number", "int_startnummer", ColumnType::Integer, 0, true);
    rating->addColumn("noncompetitive", "bol_ak", ColumnType::Boolean, 0, true, "false");
    rating->addColumn("ill", "bol_startet_nicht", ColumnType::Boolean, 0, true, "false");
    rating->addColumn("squad", "var_riege", ColumnType::Varchar, 10);
    rating->addColumn("comment", "var_comment", ColumnType::Varchar, 150);

    return rating;
}

const DBTable *Rating::m_mapping = Rating::initializeMapping();

const DBTable *Rating::mapping()
{
    return m_mapping;
}

int Rating::id() const
{
    return m_id;
}

void Rating::setId(int id)
{
    m_id = id;
}



