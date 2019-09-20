#include "dbchecker.h"
#include "dbcolumn.h"
#include "dbconstraint.h"
#include "dbtable.h"
#include "entity/abstractconnection.h"
#include "entity/event.h"

DBChecker::DBChecker(AbstractConnection *connection, QObject *parent)
    : QThread(parent)
    , connection(connection)
{}

void DBChecker::run()
{
    bool connectionEstablished = connection->connect("checker");
    if (!connectionEstablished) {
        return;
    }

    QList<DBTable *> tables = tableData();

    emit tableCount(tables.size());

    for (int i=0;i<tables.size();i++)
    {
        DBTable *table = tables.at(i);
        emit currentTable(
            tr("Überprüfe %1... %2 von %3").arg(table->name()).arg(i + 1).arg(tables.size()));
        // emit columnFKCount(table->columnFKCount());
        connect(table, SIGNAL(columnsChecked(int)), this, SLOT(columnUpdate(int)));
        table->check("checker");
        emit tablesChecked(i+1);
    }

    emit currentTable("Datenbanküberprüfung/-aktualisierung erfolgreich abgeschlossen.");

    connection->close("checker");
}

void DBChecker::columnUpdate(int count)
{
    emit columnsChecked(count);
}

QList<DBTable *> DBChecker::tableData()
{
    //    DBTable *teamPenalty = new DBTable("tfx_man_x_man_ab");
    //    teamPenalty->addColumn("int_man_x_man_abid", ColumnType::Integer, 0, false, "", "", true);
    //    teamPenalty->addColumn("int_mannschaftenid", ColumnType::Integer, 0, false);
    //    teamPenalty->addColumn("int_mannschaften_abzugid", ColumnType::Integer, 0, false);
    //    teamPenalty->addContraint("fky_mannschaften_abzugid",
    //                              "tfx_mannschaften_abzug",
    //                              "int_mannschaften_abzugid",
    //                              "int_mannschaften_abzugid",
    //                              "RESTRICT",
    //                              "RESTRICT");
    //    teamPenalty->addContraint("fky_mannschaftenid",
    //                              "tfx_mannschaften",
    //                              "int_mannschaftenid",
    //                              "int_mannschaftenid",
    //                              "RESTRICT",
    //                              "CASCADE");

    //    DBTable *teamMember = new DBTable("tfx_man_x_teilnehmer");
    //    teamMember->addColumn("int_man_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    //    teamMember->addColumn("int_mannschaftenid", ColumnType::Integer, 0, false);
    //    teamMember->addColumn("int_teilnehmerid", ColumnType::Integer, 0, false);
    //    teamMember->addColumn("int_runde", ColumnType::SmallInt);
    //    teamMember->addContraint("fky_mannschaftenid",
    //                             "tfx_mannschaften",
    //                             "int_mannschaftenid",
    //                             "int_mannschaftenid",
    //                             "RESTRICT",
    //                             "CASCADE");
    //    teamMember->addContraint("fky_teilnehmerid",
    //                             "tfx_teilnehmer",
    //                             "int_teilnehmerid",
    //                             "int_teilnehmerid",
    //                             "RESTRICT",
    //                             "RESTRICT");

    //    DBTable *groupMember = new DBTable("tfx_gruppen_x_teilnehmer");
    //    groupMember->addColumn("int_gruppen_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    //    groupMember->addColumn("int_gruppenid", ColumnType::Integer, 0, false);
    //    groupMember->addColumn("int_teilnehmerid", ColumnType::Integer, 0, false);
    //    groupMember->addContraint("fky_gruppenid",
    //                              "tfx_gruppen",
    //                              "int_gruppenid",
    //                              "int_gruppenid",
    //                              "RESTRICT",
    //                              "CASCADE");
    //    groupMember->addContraint("fky_teilnehmerid",
    //                              "tfx_teilnehmer",
    //                              "int_teilnehmerid",
    //                              "int_teilnehmerid",
    //                              "RESTRICT",
    //                              "RESTRICT");

    //    DBTable *squadDiscipline = new DBTable("tfx_riegen_x_disziplinen");
    //    squadDiscipline
    //        ->addColumn("int_riegen_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    //    squadDiscipline->addColumn("int_veranstaltungenid", ColumnType::Integer, 0, false);
    //    squadDiscipline->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    //    squadDiscipline->addColumn("int_statusid", ColumnType::Integer, 0, false);
    //    squadDiscipline->addColumn("var_riege", ColumnType::Varchar, 5);
    //    squadDiscipline->addColumn("int_runde", ColumnType::SmallInt);
    //    squadDiscipline->addColumn("bol_erstes_geraet", ColumnType::Boolean, 0, true, "'false'");
    //    squadDiscipline->addContraint("fky_disziplinenid",
    //                                  "tfx_disziplinen",
    //                                  "int_disziplinenid",
    //                                  "int_disziplinenid",
    //                                  "RESTRICT",
    //                                  "CASCADE");
    //    squadDiscipline->addContraint("fky_statusid",
    //                                  "tfx_status",
    //                                  "int_statusid",
    //                                  "int_statusid",
    //                                  "RESTRICT",
    //                                  "RESTRICT");
    //    squadDiscipline->addContraint("fky_veranstaltungenid",
    //                                  "tfx_veranstaltungen",
    //                                  "int_veranstaltungenid",
    //                                  "int_veranstaltungenid",
    //                                  "RESTRICT",
    //                                  "CASCADE");

    //    DBTable *judgement = new DBTable("tfx_wertungen");
    //    judgement->addColumn("int_wertungenid", ColumnType::Integer, 0, false, "", "", true);
    //    judgement->addColumn("int_wettkaempfeid", ColumnType::Integer, 0, false);
    //    judgement->addColumn("int_teilnehmerid", ColumnType::Integer);
    //    judgement->addColumn("int_gruppenid", ColumnType::Integer);
    //    judgement->addColumn("int_mannschaftenid", ColumnType::Integer);
    //    judgement->addColumn("int_statusid", ColumnType::Integer, 0, false);
    //    judgement->addColumn("int_runde", ColumnType::SmallInt, 0, true, "1");
    //    judgement->addColumn("int_startnummer", ColumnType::Integer);
    //    judgement->addColumn("bol_ak", ColumnType::Boolean, 0, true, "'false'");
    //    judgement->addColumn("bol_startet_nicht", ColumnType::Boolean, 0, true, "'false'");
    //    judgement->addColumn("var_riege", ColumnType::Varchar, 5);
    //    judgement->addColumn("var_comment", ColumnType::Varchar, 150);
    //    judgement->addContraint("fky_gruppenid",
    //                            "tfx_gruppen",
    //                            "int_gruppenid",
    //                            "int_gruppenid",
    //                            "RESTRICT",
    //                            "CASCADE");
    //    judgement->addContraint("fky_mannschaftenid",
    //                            "tfx_mannschaften",
    //                            "int_mannschaftenid",
    //                            "int_mannschaftenid",
    //                            "RESTRICT",
    //                            "CASCADE");
    //    judgement->addContraint("fky_statusid",
    //                            "tfx_status",
    //                            "int_statusid",
    //                            "int_statusid",
    //                            "RESTRICT",
    //                            "RESTRICT");
    //    judgement->addContraint("fky_teilnehmerid",
    //                            "tfx_teilnehmer",
    //                            "int_teilnehmerid",
    //                            "int_teilnehmerid",
    //                            "RESTRICT",
    //                            "RESTRICT");
    //    judgement->addContraint("fky_wettkaempfeid",
    //                            "tfx_wettkaempfe",
    //                            "int_wettkaempfeid",
    //                            "int_wettkaempfeid",
    //                            "RESTRICT",
    //                            "CASCADE");

    //    DBTable *judgementDetail = new DBTable("tfx_wertungen_details");
    //    judgementDetail
    //        ->addColumn("int_wertungen_detailsid", ColumnType::Integer, 0, false, "", "", true);
    //    judgementDetail->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    //    judgementDetail->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    //    judgementDetail->addColumn("int_versuch", ColumnType::SmallInt);
    //    judgementDetail->addColumn("rel_leistung", ColumnType::Real);
    //    judgementDetail->addColumn("int_kp", ColumnType::SmallInt, 0, true, "0");
    //    judgementDetail->addContraint("fky_disziplinenid",
    //                                  "tfx_disziplinen",
    //                                  "int_disziplinenid",
    //                                  "int_disziplinenid",
    //                                  "RESTRICT",
    //                                  "RESTRICT");
    //    judgementDetail->addContraint("fky_wertungenid",
    //                                  "tfx_wertungen",
    //                                  "int_wertungenid",
    //                                  "int_wertungenid",
    //                                  "RESTRICT",
    //                                  "CASCADE");

    //    DBTable *judgementDisciplin = new DBTable("tfx_wertungen_x_disziplinen");
    //    judgementDisciplin
    //        ->addColumn("int_wertungen_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    //    judgementDisciplin->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    //    judgementDisciplin->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    //    judgementDisciplin->addContraint("fky_disziplinenid",
    //                                     "tfx_disziplinen",
    //                                     "int_disziplinenid",
    //                                     "int_disziplinenid",
    //                                     "RESTRICT",
    //                                     "RESTRICT");
    //    judgementDisciplin->addContraint("fky_wertungenid",
    //                                     "tfx_wertungen",
    //                                     "int_wertungenid",
    //                                     "int_wertungenid",
    //                                     "RESTRICT",
    //                                     "CASCADE");

    //    DBTable *juryJudgement = new DBTable("tfx_jury_results");
    //    juryJudgement->addColumn("int_juryresultsid", ColumnType::Integer, 0, false, "", "", true);
    //    juryJudgement->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    //    juryJudgement->addColumn("int_disziplinen_felderid", ColumnType::Integer, 0, false);
    //    juryJudgement->addColumn("int_versuch", ColumnType::SmallInt);
    //    juryJudgement->addColumn("rel_leistung", ColumnType::Real);
    //    juryJudgement->addColumn("int_kp", ColumnType::SmallInt);
    //    juryJudgement->addContraint("fky_disziplinen_felderid",
    //                                "tfx_disziplinen_felder",
    //                                "int_disziplinen_felderid",
    //                                "int_disziplinen_felderid",
    //                                "RESTRICT",
    //                                "RESTRICT");
    //    juryJudgement->addContraint("fky_wertungenid",
    //                                "tfx_wertungen",
    //                                "int_wertungenid",
    //                                "int_wertungenid",
    //                                "RESTRICT",
    //                                "CASCADE");

    //    DBTable *layout = new DBTable("tfx_layouts");
    //    layout->addColumn("int_layoutid", ColumnType::Integer, 0, false, "", "", true);
    //    layout->addColumn("var_name", ColumnType::Varchar, 100);
    //    layout->addColumn("txt_comment", ColumnType::Text);

    //    DBTable *layoutField = new DBTable("tfx_layout_felder");
    //    layoutField->addColumn("int_layout_felderid", ColumnType::Integer, 0, false, "", "", true);
    //    layoutField->addColumn("int_layoutid", ColumnType::Integer, 0, false);
    //    layoutField->addColumn("int_typ", ColumnType::SmallInt);
    //    layoutField->addColumn("var_font", ColumnType::Varchar, 150);
    //    layoutField->addColumn("rel_x", ColumnType::Real);
    //    layoutField->addColumn("rel_y", ColumnType::Real);
    //    layoutField->addColumn("rel_w", ColumnType::Real);
    //    layoutField->addColumn("rel_h", ColumnType::Real);
    //    layoutField->addColumn("var_value", ColumnType::Varchar, 200);
    //    layoutField->addColumn("int_align", ColumnType::SmallInt, 0, true, "0");
    //    layoutField->addColumn("int_layer", ColumnType::SmallInt);
    //    layoutField->addContraint("fky_layoutid",
    //                              "tfx_layouts",
    //                              "int_layoutid",
    //                              "int_layoutid",
    //                              "RESTRICT",
    //                              "CASCADE");

    //    DBTable *startingOrder = new DBTable("tfx_startreihenfolge");
    //    startingOrder->addColumn("int_startreihenfolgeid", ColumnType::Integer, 0, false, "", "", true);
    //    startingOrder->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    //    startingOrder->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    //    startingOrder->addColumn("int_pos", ColumnType::SmallInt);
    //    startingOrder->addColumn("int_kp", ColumnType::SmallInt, 0, true, "0");
    //    startingOrder->addContraint("fky_wertungenid",
    //                                "tfx_wertungen",
    //                                "int_wertungenid",
    //                                "int_wertungenid",
    //                                "RESTRICT",
    //                                "RESTRICT");
    //    startingOrder->addContraint("fky_disziplinen",
    //                                "tfx_disziplinen",
    //                                "int_disziplinenid",
    //                                "int_disziplinenid",
    //                                "RESTRICT",
    //                                "CASCADE");

    QList<DBTable *> tables;

    //    tables.append(section);
    //    tables.append(sport);
    //    tables.append(formula);
    //    tables.append(discipline);
    //    tables.append(disciplineFields);

    //    tables.append(country);
    //    tables.append(organisation);
    //    tables.append(district);
    //    tables.append(person);
    //    tables.append(club);

    //    tables.append(group);
    //    tables.append(participant);
    //    tables.append(account);
    //    tables.append(status);
    //    tables.append(location);

    //    tables.append(Event::mapping());
    //    tables.append(competition);
    //    tables.append(competitionDiscipline);
    //    tables.append(competitionDisciplinPosition);
    //    tables.append(team);

    //    tables.append(penalty);
    //    tables.append(teamPenalty);
    //    tables.append(teamMember);
    //    tables.append(groupMember);
    //    tables.append(squadDiscipline);

    //    tables.append(judgement);
    //    tables.append(judgementDetail);
    //    tables.append(judgementDisciplin);
    //    tables.append(juryJudgement);
    //    tables.append(layout);

    //    tables.append(layoutField);
    //    tables.append(disciplinGroup);
    //    tables.append(disciplinGroupItem);
    //    tables.append(startingOrder);

    return tables;
}
