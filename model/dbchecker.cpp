#include "dbchecker.h"
#include "dbcolumn.h"
#include "dbconstraint.h"
#include "dbtable.h"
#include "entity/abstractconnection.h"

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
        emit columnFKCount(table->columnFKCount());
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
    DBTable *section = new DBTable("tfx_bereiche");
    section->addColumn("int_bereicheid", ColumnType::Integer, 0, false, "", "", true);
    section->addColumn("var_name", ColumnType::Varchar, 150, true);
    section->addColumn("bol_maennlich", ColumnType::Boolean, 0, true, "'true'");
    section->addColumn("bol_weiblich", ColumnType::Boolean, 0, true, "'true'");

    DBTable *sport = new DBTable("tfx_sport");
    sport->addColumn("int_sportid", ColumnType::Integer, 0, false, "", "", true);
    sport->addColumn("var_name", ColumnType::Varchar, 100);

    DBTable *formula = new DBTable("tfx_formeln");
    formula->addColumn("int_formelid", ColumnType::Integer, 0, false, "", "", true);
    formula->addColumn("var_name", ColumnType::Varchar, 100);
    formula->addColumn("var_formel", ColumnType::Varchar, 200);
    formula->addColumn("int_typ", ColumnType::SmallInt, 0, true, "0");

    DBTable *discipline = new DBTable("tfx_disziplinen");
    discipline->addColumn("int_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    discipline->addColumn("int_sportid", ColumnType::Integer, 0, false);
    discipline->addColumn("var_name", ColumnType::Varchar, 100);
    discipline->addColumn("var_kurz1", ColumnType::Varchar, 6);
    discipline->addColumn("var_kurz2", ColumnType::Varchar, 20);
    discipline->addColumn("var_formel", ColumnType::Varchar, 300);
    discipline->addColumn("var_maske", ColumnType::Varchar, 10);
    discipline->addColumn("int_versuche", ColumnType::Integer, 0, true, "1");
    discipline->addColumn("var_icon", ColumnType::Varchar, 50);
    discipline->addColumn("var_kuerzel", ColumnType::Varchar, 50);
    discipline->addColumn("int_berechnung", ColumnType::SmallInt, 0, true, "2");
    discipline->addColumn("var_einheit", ColumnType::Varchar, 5);
    discipline->addColumn("bol_bahnen", ColumnType::Boolean, 0, true, "'false'");
    discipline->addColumn("bol_m", ColumnType::Boolean, 0, true, "'true'");
    discipline->addColumn("bol_w", ColumnType::Boolean, 0, true, "'true'");
    discipline->addColumn("int_formelid", ColumnType::Integer, 0, true, "", " USING NULL");
    discipline->addColumn("bol_berechnen", ColumnType::Boolean, 0, true, "'true'");
    discipline->addContraint("fky_sportid",
                             "tfx_sport",
                             "int_sportid",
                             "int_sportid",
                             "RESTRICT",
                             "RESTRICT");
    discipline->addContraint("fky_formelid",
                             "tfx_formeln",
                             "int_formelid",
                             "int_formelid",
                             "RESTRICT",
                             "RESTRICT");

    DBTable *disciplineFields = new DBTable("tfx_disziplinen_felder");
    disciplineFields
        ->addColumn("int_disziplinen_felderid", ColumnType::Integer, 0, false, "", "", true);
    disciplineFields->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    disciplineFields->addColumn("var_name", ColumnType::Varchar, 15);
    disciplineFields->addColumn("int_sortierung", ColumnType::SmallInt);
    disciplineFields->addColumn("bol_endwert", ColumnType::Boolean, 0, true, "'true'");
    disciplineFields->addColumn("bol_ausgangswert", ColumnType::Boolean, 0, true, "'true'");
    disciplineFields->addColumn("int_gruppe", ColumnType::SmallInt, 0, true, "1");
    disciplineFields->addColumn("bol_enabled", ColumnType::Boolean, 0, true, "'true'");
    disciplineFields->addContraint("fky_disziplinenid",
                                   "tfx_disziplinen",
                                   "int_disziplinenid",
                                   "int_disziplinenid",
                                   "RESTRICT",
                                   "CASCADE");

    DBTable *country = new DBTable("tfx_laender");
    country->addColumn("int_laenderid", ColumnType::Integer, 0, false, "", "", true);
    country->addColumn("var_name", ColumnType::Varchar, 150);
    country->addColumn("var_kuerzel", ColumnType::Varchar, 4);

    DBTable *organisation = new DBTable("tfx_verbaende");
    organisation->addColumn("int_verbaendeid", ColumnType::Integer, 0, false, "", "", true);
    organisation->addColumn("int_laenderid", ColumnType::Integer, 0, false);
    organisation->addColumn("var_name", ColumnType::Varchar, 150);
    organisation->addColumn("var_kuerzel", ColumnType::Varchar, 8);
    organisation->addContraint("fky_laenderid",
                               "tfx_laender",
                               "int_laenderid",
                               "int_laenderid",
                               "RESTRICT",
                               "RESTRICT");

    DBTable *district = new DBTable("tfx_gaue");
    district->addColumn("int_gaueid", ColumnType::Integer, 0, false, "", "", true);
    district->addColumn("int_verbaendeid", ColumnType::Integer, 0, false);
    district->addColumn("var_name", ColumnType::Varchar, 150);
    district->addColumn("var_kuerzel", ColumnType::Varchar, 15);
    district->addContraint("fky_verbaendeid",
                           "tfx_verbaende",
                           "int_verbaendeid",
                           "int_verbaendeid",
                           "RESTRICT",
                           "RESTRICT");

    DBTable *person = new DBTable("tfx_personen");
    person->addColumn("int_personenid", ColumnType::Integer, 0, false, "", "", true);
    person->addColumn("var_vorname", ColumnType::Varchar, 150);
    person->addColumn("var_nachname", ColumnType::Varchar, 150);
    person->addColumn("var_adresse", ColumnType::Varchar, 200);
    person->addColumn("var_plz", ColumnType::Varchar, 5);
    person->addColumn("var_ort", ColumnType::Varchar, 150);
    person->addColumn("var_telefon", ColumnType::Varchar, 25);
    person->addColumn("var_fax", ColumnType::Varchar, 25);
    person->addColumn("var_email", ColumnType::Varchar, 200);

    DBTable *club = new DBTable("tfx_vereine");
    club->addColumn("int_vereineid", ColumnType::Integer, 0, false, "", "", true);
    club->addColumn("int_personenid", ColumnType::Integer);
    club->addColumn("var_name", ColumnType::Varchar, 150);
    club->addColumn("int_start_ort", ColumnType::SmallInt, 0, true, "0");
    club->addColumn("var_website", ColumnType::Varchar, 200);
    club->addColumn("int_gaueid", ColumnType::Integer, 0, false, "1");
    club->addContraint("fky_gaueid", "tfx_gaue", "int_gaueid", "int_gaueid", "RESTRICT", "RESTRICT");
    club->addContraint("fky_personenid",
                       "tfx_personen",
                       "int_personenid",
                       "int_personenid",
                       "RESTRICT",
                       "RESTRICT");

    DBTable *group = new DBTable("tfx_gruppen");
    group->addColumn("int_gruppenid", ColumnType::Integer, 0, false, "", "", true);
    group->addColumn("int_vereineid", ColumnType::Integer, 0, false);
    group->addColumn("var_name", ColumnType::Varchar, 150);

    DBTable *participant = new DBTable("tfx_teilnehmer");
    participant->addColumn("int_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    participant->addColumn("int_vereineid", ColumnType::Integer, 0, false);
    participant->addColumn("var_vorname", ColumnType::Varchar, 150);
    participant->addColumn("var_nachname", ColumnType::Varchar, 150);
    participant->addColumn("int_geschlecht", ColumnType::SmallInt, 0, false);
    participant->addColumn("dat_geburtstag", ColumnType::Date);
    participant->addColumn("bool_nur_jahr", ColumnType::Boolean, 0, true, "'true'");
    participant->addColumn("int_startpassnummer", ColumnType::Integer, 0, true, "0");
    participant->addContraint("fky_vereineid",
                              "tfx_vereine",
                              "int_vereineid",
                              "int_vereineid",
                              "RESTRICT",
                              "RESTRICT");

    DBTable *account = new DBTable("tfx_konten");
    account->addColumn("int_kontenid", ColumnType::Integer, 0, false, "", "", true);
    account->addColumn("var_name", ColumnType::Varchar, 150);
    account->addColumn("var_kontonummer", ColumnType::Varchar, 10);
    account->addColumn("var_blz", ColumnType::Varchar, 8);
    account->addColumn("var_bank", ColumnType::Varchar, 150);
    account->addColumn("var_inhabe", ColumnType::Varchar, 150);

    DBTable *status = new DBTable("tfx_status");
    status->addColumn("int_statusid", ColumnType::Integer, 0, false, "", "", true);
    status->addColumn("var_name", ColumnType::Varchar, 150);
    status->addColumn("ary_colorcode", ColumnType::Varchar, 25);
    status->addColumn("bol_bogen", ColumnType::Boolean, 0, true, "'true'");
    status->addColumn("bol_karte", ColumnType::Boolean, 0, true, "'true'");

    DBTable *location = new DBTable("tfx_wettkampforte");
    location->addColumn("int_wettkampforteid", ColumnType::Integer, 0, false, "", "", true);
    location->addColumn("var_name", ColumnType::Varchar, 150);
    location->addColumn("var_adresse", ColumnType::Varchar, 200);
    location->addColumn("var_plz", ColumnType::Varchar, 5);
    location->addColumn("var_ort", ColumnType::Varchar, 150);

    DBTable *event = new DBTable("tfx_veranstaltungen");
    event->addColumn("int_veranstaltungenid", ColumnType::Integer, 0, false, "", "", true);
    event->addColumn("int_wettkampforteid", ColumnType::Integer, 0, false);
    event->addColumn("int_meldung_an", ColumnType::Integer);
    event->addColumn("int_ansprechpartner", ColumnType::Integer);
    event->addColumn("int_kontenid", ColumnType::Integer);
    event->addColumn("int_hauptwettkampf", ColumnType::Integer);
    event->addColumn("var_name", ColumnType::Varchar, 250);
    event->addColumn("int_runde", ColumnType::SmallInt, 0, true, "1");
    event->addColumn("dat_von", ColumnType::Date, 0, false);
    event->addColumn("dat_bis", ColumnType::Date, 0, false);
    event->addColumn("dat_meldeschluss", ColumnType::Date);
    event->addColumn("bol_rundenwettkampf", ColumnType::Boolean, 0, true, "'false'");
    event->addColumn("var_veranstalter", ColumnType::Varchar, 150);
    event->addColumn("int_edv", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("int_helfer", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("int_kampfrichter", ColumnType::SmallInt, 0, true, "0");
    event->addColumn("var_meldung_website", ColumnType::Varchar, 200);
    event->addColumn("var_verwendungszweck", ColumnType::Varchar, 150);
    event->addColumn("rel_meldegeld", ColumnType::Real, 0, true, "0");
    event->addColumn("rel_nachmeldung", ColumnType::Real, 0, true, "0");
    event->addColumn("bol_faellig_nichtantritt", ColumnType::Boolean, 0, true, "'false'");
    event->addColumn("bol_ummeldung_moeglich", ColumnType::Boolean, 0, true, "'false'");
    event->addColumn("bol_nachmeldung_moeglich", ColumnType::Boolean, 0, true, "'false'");
    event->addColumn("txt_meldung_an", ColumnType::Text);
    event->addColumn("txt_startberechtigung", ColumnType::Text);
    event->addColumn("txt_teilnahmebedingungen", ColumnType::Text);
    event->addColumn("txt_siegerauszeichnung", ColumnType::Text);
    event->addColumn("txt_kampfrichter", ColumnType::Text);
    event->addColumn("txt_hinweise", ColumnType::Text);
    event->addContraint("fky_ansprechpartner",
                        "tfx_personen",
                        "int_ansprechpartner",
                        "int_personenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_hauptwettkampf",
                        "tfx_veranstaltungen",
                        "int_hauptwettkampf",
                        "int_veranstaltungenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_kontenid",
                        "tfx_konten",
                        "int_kontenid",
                        "int_kontenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_meldung_an",
                        "tfx_personen",
                        "int_meldung_an",
                        "int_personenid",
                        "RESTRICT",
                        "RESTRICT");
    event->addContraint("fky_wettkampforteid",
                        "tfx_wettkampforte",
                        "int_wettkampforteid",
                        "int_wettkampforteid",
                        "RESTRICT",
                        "RESTRICT");

    DBTable *competition = new DBTable("tfx_wettkaempfe");
    competition->addColumn("int_wettkaempfeid", ColumnType::Integer, 0, false, "", "", true);
    competition->addColumn("int_veranstaltungenid", ColumnType::Integer, 0, false);
    competition->addColumn("int_bereicheid", ColumnType::Integer, 0, false);
    competition->addColumn("int_typ", ColumnType::SmallInt, 0, true, "0");
    competition->addColumn("var_nummer", ColumnType::Varchar, 5);
    competition->addColumn("var_name", ColumnType::Varchar, 150);
    competition->addColumn("yer_von", ColumnType::SmallInt, 0, false);
    competition->addColumn("yer_bis", ColumnType::SmallInt);
    competition->addColumn("int_qualifikation", ColumnType::SmallInt, 0, true, "0");
    competition->addColumn("int_wertungen", ColumnType::SmallInt);
    competition->addColumn("bol_streichwertung", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_ak_anzeigen", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_wahlwettkampf", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("int_durchgang", ColumnType::SmallInt, 0, true, "1");
    competition->addColumn("int_bahn", ColumnType::SmallInt, 0, true, "1");
    competition->addColumn("tim_startzeit", ColumnType::TimeNoTimezone);
    competition->addColumn("tim_einturnen", ColumnType::TimeNoTimezone);
    competition->addColumn("bol_info_anzeigen", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_kp", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_sortasc", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_mansort", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("bol_gerpkt", ColumnType::Boolean, 0, true, "'false'");
    competition->addColumn("int_anz_streich", ColumnType::SmallInt, 0, true, "0");
    competition->addContraint("fky_bereicheid",
                              "tfx_bereiche",
                              "int_bereicheid",
                              "int_bereicheid",
                              "RESTRICT",
                              "RESTRICT");
    competition->addContraint("fky_veranstaltungenid",
                              "tfx_veranstaltungen",
                              "int_veranstaltungenid",
                              "int_veranstaltungenid",
                              "RESTRICT",
                              "CASCADE");

    DBTable *competitionDiscipline = new DBTable("tfx_wettkaempfe_x_disziplinen");
    competitionDiscipline
        ->addColumn("int_wettkaempfe_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    competitionDiscipline->addColumn("int_wettkaempfeid", ColumnType::Integer, 0, false);
    competitionDiscipline->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    competitionDiscipline->addColumn("var_ausschreibung", ColumnType::Varchar, 100);
    competitionDiscipline->addColumn("int_sortierung", ColumnType::SmallInt);
    competitionDiscipline->addColumn("bol_kp", ColumnType::Boolean, 0, true, "'false'");
    competitionDiscipline->addColumn("rel_max", ColumnType::Real, 0, true, "0");
    competitionDiscipline->addContraint("fky_disziplinenid",
                                        "tfx_disziplinen",
                                        "int_disziplinenid",
                                        "int_disziplinenid",
                                        "RESTRICT",
                                        "RESTRICT");
    competitionDiscipline->addContraint("fky_wettkaempfeid",
                                        "tfx_wettkaempfe",
                                        "int_wettkaempfeid",
                                        "int_wettkaempfeid",
                                        "RESTRICT",
                                        "CASCADE");

    DBTable *competitionDisciplinPosition = new DBTable("tfx_wettkaempfe_dispos");
    competitionDisciplinPosition
        ->addColumn("int_wettkaempfe_disposid", ColumnType::Integer, 0, false, "", "", true);
    competitionDisciplinPosition->addColumn("int_wettkaempfe_x_disziplinenid",
                                            ColumnType::Integer,
                                            0,
                                            false);
    competitionDisciplinPosition->addColumn("int_sortx", ColumnType::SmallInt, 0, true, "0");
    competitionDisciplinPosition->addColumn("int_sorty", ColumnType::SmallInt, 0, true, "0");
    competitionDisciplinPosition->addColumn("int_kp", ColumnType::SmallInt, 0, true, "0");
    competitionDisciplinPosition->addContraint("fky_wettkaempfe_x_disziplinenid",
                                               "tfx_wettkaempfe_x_disziplinen",
                                               "int_wettkaempfe_x_disziplinenid",
                                               "int_wettkaempfe_x_disziplinenid",
                                               "RESTRICT",
                                               "RESTRICT");

    DBTable *team = new DBTable("tfx_mannschaften");
    team->addColumn("int_mannschaftenid", ColumnType::Integer, 0, false, "", "", true);
    team->addColumn("int_wettkaempfeid", ColumnType::Integer, 0, false);
    team->addColumn("int_vereineid", ColumnType::Integer, 0, false);
    team->addColumn("int_nummer", ColumnType::SmallInt, 0, true, "1");
    team->addColumn("var_riege", ColumnType::Varchar, 5, true, "1");
    team->addColumn("int_startnummer", ColumnType::Integer);
    team->addContraint("fky_vereineid",
                       "tfx_vereine",
                       "int_vereineid",
                       "int_vereineid",
                       "RESTRICT",
                       "RESTRICT");
    team->addContraint("fky_wettkaempfeid",
                       "tfx_wettkaempfe",
                       "int_wettkaempfeid",
                       "int_wettkaempfeid",
                       "RESTRICT",
                       "CASCADE");

    DBTable *penalty = new DBTable("tfx_mannschaften_abzug");
    penalty->addColumn("int_mannschaften_abzugid", ColumnType::Integer, 0, false, "", "", true);
    penalty->addColumn("var_name", ColumnType::Varchar, 100);
    penalty->addColumn("rel_abzug", ColumnType::Real);

    DBTable *teamPenalty = new DBTable("tfx_man_x_man_ab");
    teamPenalty->addColumn("int_man_x_man_abid", ColumnType::Integer, 0, false, "", "", true);
    teamPenalty->addColumn("int_mannschaftenid", ColumnType::Integer, 0, false);
    teamPenalty->addColumn("int_mannschaften_abzugid", ColumnType::Integer, 0, false);
    teamPenalty->addContraint("fky_mannschaften_abzugid",
                              "tfx_mannschaften_abzug",
                              "int_mannschaften_abzugid",
                              "int_mannschaften_abzugid",
                              "RESTRICT",
                              "RESTRICT");
    teamPenalty->addContraint("fky_mannschaftenid",
                              "tfx_mannschaften",
                              "int_mannschaftenid",
                              "int_mannschaftenid",
                              "RESTRICT",
                              "CASCADE");

    DBTable *teamMember = new DBTable("tfx_man_x_teilnehmer");
    teamMember->addColumn("int_man_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    teamMember->addColumn("int_mannschaftenid", ColumnType::Integer, 0, false);
    teamMember->addColumn("int_teilnehmerid", ColumnType::Integer, 0, false);
    teamMember->addColumn("int_runde", ColumnType::SmallInt);
    teamMember->addContraint("fky_mannschaftenid",
                             "tfx_mannschaften",
                             "int_mannschaftenid",
                             "int_mannschaftenid",
                             "RESTRICT",
                             "CASCADE");
    teamMember->addContraint("fky_teilnehmerid",
                             "tfx_teilnehmer",
                             "int_teilnehmerid",
                             "int_teilnehmerid",
                             "RESTRICT",
                             "RESTRICT");

    DBTable *groupMember = new DBTable("tfx_gruppen_x_teilnehmer");
    groupMember->addColumn("int_gruppen_x_teilnehmerid", ColumnType::Integer, 0, false, "", "", true);
    groupMember->addColumn("int_gruppenid", ColumnType::Integer, 0, false);
    groupMember->addColumn("int_teilnehmerid", ColumnType::Integer, 0, false);
    groupMember->addContraint("fky_gruppenid",
                              "tfx_gruppen",
                              "int_gruppenid",
                              "int_gruppenid",
                              "RESTRICT",
                              "CASCADE");
    groupMember->addContraint("fky_teilnehmerid",
                              "tfx_teilnehmer",
                              "int_teilnehmerid",
                              "int_teilnehmerid",
                              "RESTRICT",
                              "RESTRICT");

    DBTable *squadDiscipline = new DBTable("tfx_riegen_x_disziplinen");
    squadDiscipline
        ->addColumn("int_riegen_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    squadDiscipline->addColumn("int_veranstaltungenid", ColumnType::Integer, 0, false);
    squadDiscipline->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    squadDiscipline->addColumn("int_statusid", ColumnType::Integer, 0, false);
    squadDiscipline->addColumn("var_riege", ColumnType::Varchar, 5);
    squadDiscipline->addColumn("int_runde", ColumnType::SmallInt);
    squadDiscipline->addColumn("bol_erstes_geraet", ColumnType::Boolean, 0, true, "'false'");
    squadDiscipline->addContraint("fky_disziplinenid",
                                  "tfx_disziplinen",
                                  "int_disziplinenid",
                                  "int_disziplinenid",
                                  "RESTRICT",
                                  "CASCADE");
    squadDiscipline->addContraint("fky_statusid",
                                  "tfx_status",
                                  "int_statusid",
                                  "int_statusid",
                                  "RESTRICT",
                                  "RESTRICT");
    squadDiscipline->addContraint("fky_veranstaltungenid",
                                  "tfx_veranstaltungen",
                                  "int_veranstaltungenid",
                                  "int_veranstaltungenid",
                                  "RESTRICT",
                                  "CASCADE");

    DBTable *judgement = new DBTable("tfx_wertungen");
    judgement->addColumn("int_wertungenid", ColumnType::Integer, 0, false, "", "", true);
    judgement->addColumn("int_wettkaempfeid", ColumnType::Integer, 0, false);
    judgement->addColumn("int_teilnehmerid", ColumnType::Integer);
    judgement->addColumn("int_gruppenid", ColumnType::Integer);
    judgement->addColumn("int_mannschaftenid", ColumnType::Integer);
    judgement->addColumn("int_statusid", ColumnType::Integer, 0, false);
    judgement->addColumn("int_runde", ColumnType::SmallInt, 0, true, "1");
    judgement->addColumn("int_startnummer", ColumnType::Integer);
    judgement->addColumn("bol_ak", ColumnType::Boolean, 0, true, "'false'");
    judgement->addColumn("bol_startet_nicht", ColumnType::Boolean, 0, true, "'false'");
    judgement->addColumn("var_riege", ColumnType::Varchar, 5);
    judgement->addColumn("var_comment", ColumnType::Varchar, 150);
    judgement->addContraint("fky_gruppenid",
                            "tfx_gruppen",
                            "int_gruppenid",
                            "int_gruppenid",
                            "RESTRICT",
                            "CASCADE");
    judgement->addContraint("fky_mannschaftenid",
                            "tfx_mannschaften",
                            "int_mannschaftenid",
                            "int_mannschaftenid",
                            "RESTRICT",
                            "CASCADE");
    judgement->addContraint("fky_statusid",
                            "tfx_status",
                            "int_statusid",
                            "int_statusid",
                            "RESTRICT",
                            "RESTRICT");
    judgement->addContraint("fky_teilnehmerid",
                            "tfx_teilnehmer",
                            "int_teilnehmerid",
                            "int_teilnehmerid",
                            "RESTRICT",
                            "RESTRICT");
    judgement->addContraint("fky_wettkaempfeid",
                            "tfx_wettkaempfe",
                            "int_wettkaempfeid",
                            "int_wettkaempfeid",
                            "RESTRICT",
                            "CASCADE");

    DBTable *judgementDetail = new DBTable("tfx_wertungen_details");
    judgementDetail
        ->addColumn("int_wertungen_detailsid", ColumnType::Integer, 0, false, "", "", true);
    judgementDetail->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    judgementDetail->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    judgementDetail->addColumn("int_versuch", ColumnType::SmallInt);
    judgementDetail->addColumn("rel_leistung", ColumnType::Real);
    judgementDetail->addColumn("int_kp", ColumnType::SmallInt, 0, true, "0");
    judgementDetail->addContraint("fky_disziplinenid",
                                  "tfx_disziplinen",
                                  "int_disziplinenid",
                                  "int_disziplinenid",
                                  "RESTRICT",
                                  "RESTRICT");
    judgementDetail->addContraint("fky_wertungenid",
                                  "tfx_wertungen",
                                  "int_wertungenid",
                                  "int_wertungenid",
                                  "RESTRICT",
                                  "CASCADE");

    DBTable *judgementDisciplin = new DBTable("tfx_wertungen_x_disziplinen");
    judgementDisciplin
        ->addColumn("int_wertungen_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    judgementDisciplin->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    judgementDisciplin->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    judgementDisciplin->addContraint("fky_disziplinenid",
                                     "tfx_disziplinen",
                                     "int_disziplinenid",
                                     "int_disziplinenid",
                                     "RESTRICT",
                                     "RESTRICT");
    judgementDisciplin->addContraint("fky_wertungenid",
                                     "tfx_wertungen",
                                     "int_wertungenid",
                                     "int_wertungenid",
                                     "RESTRICT",
                                     "CASCADE");

    DBTable *juryJudgement = new DBTable("tfx_jury_results");
    juryJudgement->addColumn("int_juryresultsid", ColumnType::Integer, 0, false, "", "", true);
    juryJudgement->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    juryJudgement->addColumn("int_disziplinen_felderid", ColumnType::Integer, 0, false);
    juryJudgement->addColumn("int_versuch", ColumnType::SmallInt);
    juryJudgement->addColumn("rel_leistung", ColumnType::Real);
    juryJudgement->addColumn("int_kp", ColumnType::SmallInt);
    juryJudgement->addContraint("fky_disziplinen_felderid",
                                "tfx_disziplinen_felder",
                                "int_disziplinen_felderid",
                                "int_disziplinen_felderid",
                                "RESTRICT",
                                "RESTRICT");
    juryJudgement->addContraint("fky_wertungenid",
                                "tfx_wertungen",
                                "int_wertungenid",
                                "int_wertungenid",
                                "RESTRICT",
                                "CASCADE");

    DBTable *layout = new DBTable("tfx_layouts");
    layout->addColumn("int_layoutid", ColumnType::Integer, 0, false, "", "", true);
    layout->addColumn("var_name", ColumnType::Varchar, 100);
    layout->addColumn("txt_comment", ColumnType::Text);

    DBTable *layoutField = new DBTable("tfx_layout_felder");
    layoutField->addColumn("int_layout_felderid", ColumnType::Integer, 0, false, "", "", true);
    layoutField->addColumn("int_layoutid", ColumnType::Integer, 0, false);
    layoutField->addColumn("int_typ", ColumnType::SmallInt);
    layoutField->addColumn("var_font", ColumnType::Varchar, 150);
    layoutField->addColumn("rel_x", ColumnType::Real);
    layoutField->addColumn("rel_y", ColumnType::Real);
    layoutField->addColumn("rel_w", ColumnType::Real);
    layoutField->addColumn("rel_h", ColumnType::Real);
    layoutField->addColumn("var_value", ColumnType::Varchar, 200);
    layoutField->addColumn("int_align", ColumnType::SmallInt, 0, true, "0");
    layoutField->addColumn("int_layer", ColumnType::SmallInt);
    layoutField->addContraint("fky_layoutid",
                              "tfx_layouts",
                              "int_layoutid",
                              "int_layoutid",
                              "RESTRICT",
                              "CASCADE");

    DBTable *disciplinGroup = new DBTable("tfx_disziplinen_gruppen");
    disciplinGroup
        ->addColumn("int_disziplinen_gruppenid", ColumnType::Integer, 0, false, "", "", true);
    disciplinGroup->addColumn("var_name", ColumnType::Varchar, 100);
    disciplinGroup->addColumn("txt_comment", ColumnType::Text);

    DBTable *disciplinGroupItem = new DBTable("tfx_disgrp_x_disziplinen");
    disciplinGroupItem
        ->addColumn("int_disgrp_x_disziplinenid", ColumnType::Integer, 0, false, "", "", true);
    disciplinGroupItem->addColumn("int_disziplinen_gruppenid", ColumnType::Integer, 0, false);
    disciplinGroupItem->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    disciplinGroupItem->addColumn("int_pos", ColumnType::SmallInt);
    disciplinGroupItem->addContraint("fky_disziplinen_gruppenid",
                                     "tfx_disziplinen_gruppen",
                                     "int_disziplinen_gruppenid",
                                     "int_disziplinen_gruppenid",
                                     "RESTRICT",
                                     "RESTRICT");
    disciplinGroupItem->addContraint("fky_disziplinen",
                                     "tfx_disziplinen",
                                     "int_disziplinenid",
                                     "int_disziplinenid",
                                     "RESTRICT",
                                     "CASCADE");

    DBTable *startingOrder = new DBTable("tfx_startreihenfolge");
    startingOrder->addColumn("int_startreihenfolgeid", ColumnType::Integer, 0, false, "", "", true);
    startingOrder->addColumn("int_wertungenid", ColumnType::Integer, 0, false);
    startingOrder->addColumn("int_disziplinenid", ColumnType::Integer, 0, false);
    startingOrder->addColumn("int_pos", ColumnType::SmallInt);
    startingOrder->addColumn("int_kp", ColumnType::SmallInt, 0, true, "0");
    startingOrder->addContraint("fky_wertungenid",
                                "tfx_wertungen",
                                "int_wertungenid",
                                "int_wertungenid",
                                "RESTRICT",
                                "RESTRICT");
    startingOrder->addContraint("fky_disziplinen",
                                "tfx_disziplinen",
                                "int_disziplinenid",
                                "int_disziplinenid",
                                "RESTRICT",
                                "CASCADE");

    QList<DBTable *> tables;

    tables.append(section);
    tables.append(sport);
    tables.append(formula);
    tables.append(discipline);
    tables.append(disciplineFields);

    tables.append(country);
    tables.append(organisation);
    tables.append(district);
    tables.append(person);
    tables.append(club);

    tables.append(group);
    tables.append(participant);
    tables.append(account);
    tables.append(status);
    tables.append(location);

    tables.append(event);
    tables.append(competition);
    tables.append(competitionDiscipline);
    tables.append(competitionDisciplinPosition);
    tables.append(team);

    tables.append(penalty);
    tables.append(teamPenalty);
    tables.append(teamMember);
    tables.append(groupMember);
    tables.append(squadDiscipline);

    tables.append(judgement);
    tables.append(judgementDetail);
    tables.append(judgementDisciplin);
    tables.append(juryJudgement);
    tables.append(layout);

    tables.append(layoutField);
    tables.append(disciplinGroup);
    tables.append(disciplinGroupItem);
    tables.append(startingOrder);

    return tables;
}
