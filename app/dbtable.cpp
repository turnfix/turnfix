#include "dbtable.h"
#include "dbcolumn.h"
#include "dbconstraint.h"
#include "model/database/db.h"
#include "model/database/tfsqlquery.h"
#include "model/settings/tfsettings.h"
#include <QVariant>

QList<DBTable*> DBTable::tables = QList<DBTable*>();

DBTable::DBTable(QString name, QObject *parent) : QObject(parent) {
    this->name = name;
}

void DBTable::addColumn(DBColumn *column)
{
    columns.append(column);
}

void DBTable::addContraint(DBConstraint *constraint)
{
    constraints.append(constraint);
}

QString DBTable::getName()
{
    return this->name;
}

int DBTable::getColumnFKCount()
{
    return columns.size()+constraints.size();
}

void DBTable::checkTable(DB *db)
{
    bool found;
    DBColumn *column;
    DBConstraint *constraint;
    TFSqlQuery tableExists(db);
    TFSqlQuery columnList(db);
    TFSqlQuery constraintList(db);
    TFSqlQuery change(db);
    TFSettings *settings;

    found = false;
    settings = TFSettings::getInstance();

    tableExists.prepare("SELECT TABLE WITH NAME :tablename");
    tableExists.bindValue(":tablename",this->name);
    tableExists.exec();

    if (db->count(tableExists)==0)
    {
        //Create the table
        this->create(db);
    }
    else
    {
        //Check existing table
        //Get Columns
        columnList.prepare("SELECT COLUMN INFOS FROM '"+this->name+"'");
        columnList.exec();

        //Check if all columns exist in database
        for (int i=0;i<columns.size();i++)
        {
            column = columns.at(i);
            if (columnList.next())
            {
                //Check name
                if (column->getName() != columnList.value(1).toString())
                {
                    //Change name
                    change.exec(QString("ALTER TABLE %1 RENAME %2 TO %3").arg(this->name,columnList.value(1).toString(),column->getName()));
                }

                //Check type
                if (column->getType() != toTFType(columnList.value(2).toString()))
                {
                    //Change type

                    //First take care of the default value...
                    if (column->getDefaultValue() != "") {
                        change.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 SET DEFAULT %3").arg(this->name,column->getName(),column->getDefaultValue()));
                    } else {
                        change.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 DROP DEFAULT").arg(this->name,column->getName()));
                    }

                    //...Then change the type
                    change.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 TYPE %3%4").arg(this->name,column->getName(),column->getTypeWithLength(),column->getExtraQuery()));
                }

                //Check length, but only if not SQLite
                if (column->getLength() > 0 && settings->getDbTyp() != DB::SQLite)
                {
                    if (column->getLength() != columnList.value(5).toInt())
                    {
                        //Change length
                        change.exec(QString("ALTER TABLE %1 ALTER COLUMN %2 TYPE %3").arg(this->name,column->getName(),column->getTypeWithLength()));
                    }
                }

            }
            else
            {
                //Create column
                change.exec(QString("ALTER TABLE %1 ADD COLUMN %2 %3 %4 %5").arg(this->name,column->getName(),column->getTypeWithLength(),column->getNullString(),column->getDefaultValue()==""?"":"DEFAULT "+column->getDefaultValue()));
            }
            emit columnsChecked(i+1);
        }

        //Check contraints
        constraintList.prepare("SELECT CONTRAINT INFO FROM '"+this->name+"'");
        for (int i=0;i<constraints.size();i++)
        {
            constraint = constraints.at(i);
            constraintList.exec();
            while (constraintList.next())
            {
                if (constraint->getReferenceTable() == constraintList.value(2).toString() && constraint->getToField() == constraintList.value(4).toString())
                {
                    found = true;
                }
            }
            if (!found)
            {
                //Create constraint
                change.exec(QString("ALTER TABLE %1 ADD CONSTRAINT %2 FOREIGN KEY (%3) REFERENCES %4 (%5) ON UPDATE %6 ON DELETE %7").arg(this->name,constraint->getName(),constraint->getFromField(),constraint->getReferenceTable(),constraint->getToField(),constraint->getOnUpdate(),constraint->getOnDelete()));
            }
            emit columnsChecked(columns.size()+i+1);
        }

    }
}

void DBTable::create(DB* db)
{
    TFSqlQuery create(db);
    QString query;
    int pk;
    int i;

    query = "CREATE TABLE "+this->getName()+" (";
    pk = -1;
    i = 0;

    foreach(DBColumn *column, this->columns)
    {
        query += column->getName() + " ";
        if (column->getPK())
            query += "PKFIELD";
        else
            query += column->getTypeWithLength();
        if (!column->getNull())
            query += " NOT NULL";
        if (column->getDefaultValue() != "")
            query += " DEFAULT "+column->getDefaultValue();
        query += ",";
        if (column->getPK())
            pk = i;
        i++;
    }
    if (pk != -1)
    {
        query += "CONSTRAINT pky_"+columns.at(pk)->getName()+" PRIMARY KEY ("+columns.at(pk)->getName()+"),";
    }
    foreach(DBConstraint *constraint, this->constraints)
    {
        query += "CONSTRAINT "+constraint->getName()+" FOREIGN KEY ("+constraint->getFromField()+") ";
        query += "REFERENCES "+constraint->getReferenceTable()+" ("+constraint->getToField()+") ";
        query += "ON UPDATE "+constraint->getOnUpdate()+" ON DELETE "+constraint->getOnDelete()+",";
    }

    query = query.left(query.length()-1);
    query += ")";


    create.exec(query);

}

QString DBTable::toTFType(QString type)
{
    TFSettings *settings;

    settings = TFSettings::getInstance();

    if (settings->getDbTyp() == DB::PostgreSQL)
    {
        type.replace("character varying","varchar");
    }

    return type;
}

QList<DBTable*> DBTable::getTables()
{
    return tables;
}

void DBTable::setupTables()
{
    DBTable *section = new DBTable("tfx_bereiche");
    section->addColumn(new DBColumn("int_bereicheid","integer",0,false,"","",true));
    section->addColumn(new DBColumn("var_name","varchar",150,true));
    section->addColumn(new DBColumn("bol_maennlich","boolean",0,true,"'true'"));
    section->addColumn(new DBColumn("bol_weiblich","boolean",0,true,"'true'"));

    DBTable *sport = new DBTable("tfx_sport");
    sport->addColumn(new DBColumn("int_sportid","integer",0,false,"","",true));
    sport->addColumn(new DBColumn("var_name","varchar",100));

    DBTable *formula = new DBTable("tfx_formeln");
    formula->addColumn(new DBColumn("int_formelid","integer",0,false,"","",true));
    formula->addColumn(new DBColumn("var_name","varchar",100));
    formula->addColumn(new DBColumn("var_formel","varchar",200));
    formula->addColumn(new DBColumn("int_typ","smallint",0,true,"0"));

    DBTable *discipline = new DBTable("tfx_disziplinen");
    discipline->addColumn(new DBColumn("int_disziplinenid","integer",0,false,"","",true));
    discipline->addColumn(new DBColumn("int_sportid","integer",0,false));
    discipline->addColumn(new DBColumn("var_name","varchar", 100));
    discipline->addColumn(new DBColumn("var_kurz1","varchar",6));
    discipline->addColumn(new DBColumn("var_kurz2","varchar",20));
    discipline->addColumn(new DBColumn("var_formel","varchar",300));
    discipline->addColumn(new DBColumn("var_maske","varchar",10));
    discipline->addColumn(new DBColumn("int_versuche","integer",0,true,"1"));
    discipline->addColumn(new DBColumn("var_icon","varchar",50));
    discipline->addColumn(new DBColumn("var_kuerzel","varchar",50));
    discipline->addColumn(new DBColumn("int_berechnung","smallint",0,true,"2"));
    discipline->addColumn(new DBColumn("var_einheit","varchar",5));
    discipline->addColumn(new DBColumn("bol_bahnen","boolean",0,true,"'false'"));
    discipline->addColumn(new DBColumn("bol_m","boolean",0,true,"'true'"));
    discipline->addColumn(new DBColumn("bol_w","boolean",0,true,"'true'"));
    discipline->addColumn(new DBColumn("int_formelid","integer",0,true,""," USING NULL"));
    discipline->addColumn(new DBColumn("bol_berechnen","boolean",0,true,"'true'"));
    discipline->addContraint(new DBConstraint("fky_sportid","tfx_sport","int_sportid","int_sportid","RESTRICT","RESTRICT"));
    discipline->addContraint(new DBConstraint("fky_formelid","tfx_formeln","int_formelid","int_formelid","RESTRICT","RESTRICT"));

    DBTable *disciplineFields = new DBTable("tfx_disziplinen_felder");
    disciplineFields->addColumn(new DBColumn("int_disziplinen_felderid","integer",0,false,"","",true));
    disciplineFields->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    disciplineFields->addColumn(new DBColumn("var_name","varchar",15));
    disciplineFields->addColumn(new DBColumn("int_sortierung","smallint"));
    disciplineFields->addColumn(new DBColumn("bol_endwert","boolean",0,true,"'true'"));
    disciplineFields->addColumn(new DBColumn("bol_ausgangswert","boolean",0,true,"'true'"));
    disciplineFields->addColumn(new DBColumn("int_gruppe","smallint",0,true,"1"));
    disciplineFields->addColumn(new DBColumn("bol_enabled","boolean",0,true,"'true'"));
    disciplineFields->addContraint(new DBConstraint("fky_disziplinenid","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","CASCADE"));

    DBTable *country = new DBTable("tfx_laender");
    country->addColumn(new DBColumn("int_laenderid","integer",0,false,"","",true));
    country->addColumn(new DBColumn("var_name","varchar",150));
    country->addColumn(new DBColumn("var_kuerzel","varchar",4));

    DBTable *organisation = new DBTable("tfx_verbaende");
    organisation->addColumn(new DBColumn("int_verbaendeid","integer",0,false,"","",true));
    organisation->addColumn(new DBColumn("int_laenderid","integer",0,false));
    organisation->addColumn(new DBColumn("var_name","varchar",150));
    organisation->addColumn(new DBColumn("var_kuerzel","varchar",8));
    organisation->addContraint(new DBConstraint("fky_laenderid","tfx_laender","int_laenderid","int_laenderid","RESTRICT","RESTRICT"));

    DBTable *district = new DBTable("tfx_gaue");
    district->addColumn(new DBColumn("int_gaueid","integer",0,false,"","",true));
    district->addColumn(new DBColumn("int_verbaendeid","integer",0,false));
    district->addColumn(new DBColumn("var_name","varchar",150));
    district->addColumn(new DBColumn("var_kuerzel","varchar",15));
    district->addContraint(new DBConstraint("fky_verbaendeid","tfx_verbaende","int_verbaendeid","int_verbaendeid","RESTRICT","RESTRICT"));

    DBTable *person = new DBTable("tfx_personen");
    person->addColumn(new DBColumn("int_personenid","integer",0,false,"","",true));
    person->addColumn(new DBColumn("var_vorname","varchar",150));
    person->addColumn(new DBColumn("var_nachname","varchar",150));
    person->addColumn(new DBColumn("var_adresse","varchar",200));
    person->addColumn(new DBColumn("var_plz","varchar",5));
    person->addColumn(new DBColumn("var_ort","varchar",150));
    person->addColumn(new DBColumn("var_telefon","varchar",25));
    person->addColumn(new DBColumn("var_fax","varchar",25));
    person->addColumn(new DBColumn("var_email","varchar",200));

    DBTable *club = new DBTable("tfx_vereine");
    club->addColumn(new DBColumn("int_vereineid","integer",0,false,"","",true));
    club->addColumn(new DBColumn("int_personenid","integer"));
    club->addColumn(new DBColumn("var_name","varchar",150));
    club->addColumn(new DBColumn("int_start_ort","smallint",0,true,"0"));
    club->addColumn(new DBColumn("var_website","varchar",200));
    club->addColumn(new DBColumn("int_gaueid","integer",0,false,"1"));
    club->addContraint(new DBConstraint("fky_gaueid","tfx_gaue","int_gaueid","int_gaueid","RESTRICT","RESTRICT"));
    club->addContraint(new DBConstraint("fky_personenid","tfx_personen","int_personenid","int_personenid","RESTRICT","RESTRICT"));

    DBTable *group = new DBTable("tfx_gruppen");
    group->addColumn(new DBColumn("int_gruppenid","integer",0,false,"","",true));
    group->addColumn(new DBColumn("int_vereineid","integer",0,false));
    group->addColumn(new DBColumn("var_name","varchar",150));

    DBTable *participant = new DBTable("tfx_teilnehmer");
    participant->addColumn(new DBColumn("int_teilnehmerid","integer",0,false,"","",true));
    participant->addColumn(new DBColumn("int_vereineid","integer",0,false));
    participant->addColumn(new DBColumn("var_vorname","varchar",150));
    participant->addColumn(new DBColumn("var_nachname","varchar",150));
    participant->addColumn(new DBColumn("int_geschlecht","smallint",0,false));
    participant->addColumn(new DBColumn("dat_geburtstag","date"));
    participant->addColumn(new DBColumn("bool_nur_jahr","boolean",0,true,"'true'"));
    participant->addColumn(new DBColumn("int_startpassnummer","integer",0,true,"0"));
    participant->addContraint(new DBConstraint("fky_vereineid","tfx_vereine","int_vereineid","int_vereineid","RESTRICT","RESTRICT"));

    DBTable *account = new DBTable("tfx_konten");
    account->addColumn(new DBColumn("int_kontenid","integer",0,false,"","",true));
    account->addColumn(new DBColumn("var_name","varchar",150));
    account->addColumn(new DBColumn("var_kontonummer","varchar",10));
    account->addColumn(new DBColumn("var_blz","varchar",8));
    account->addColumn(new DBColumn("var_bank","varchar",150));
    account->addColumn(new DBColumn("var_inhabe","varchar",150));

    DBTable *status = new DBTable("tfx_status");
    status->addColumn(new DBColumn("int_statusid","integer",0,false,"","",true));
    status->addColumn(new DBColumn("var_name","varchar",150));
    status->addColumn(new DBColumn("ary_colorcode","varchar",25));
    status->addColumn(new DBColumn("bol_bogen","boolean",0,true,"'true'"));
    status->addColumn(new DBColumn("bol_karte","boolean",0,true,"'true'"));

    DBTable *location = new DBTable("tfx_wettkampforte");
    location->addColumn(new DBColumn("int_wettkampforteid","integer",0,false,"","",true));
    location->addColumn(new DBColumn("var_name","varchar",150));
    location->addColumn(new DBColumn("var_adresse","varchar",200));
    location->addColumn(new DBColumn("var_plz","varchar",5));
    location->addColumn(new DBColumn("var_ort","varchar",150));

    DBTable *event = new DBTable("tfx_veranstaltungen");
    event->addColumn(new DBColumn("int_veranstaltungenid","integer",0,false,"","",true));
    event->addColumn(new DBColumn("int_wettkampforteid","integer",0,false));
    event->addColumn(new DBColumn("int_meldung_an","integer"));
    event->addColumn(new DBColumn("int_ansprechpartner","integer"));
    event->addColumn(new DBColumn("int_kontenid","integer"));
    event->addColumn(new DBColumn("int_hauptwettkampf","integer"));
    event->addColumn(new DBColumn("var_name","varchar",250));
    event->addColumn(new DBColumn("int_runde","smallint",0,true,"1"));
    event->addColumn(new DBColumn("dat_von","date",0,false));
    event->addColumn(new DBColumn("dat_bis","date",0,false));
    event->addColumn(new DBColumn("dat_meldeschluss","date"));
    event->addColumn(new DBColumn("bol_rundenwettkampf","boolean",0,true,"'false'"));
    event->addColumn(new DBColumn("var_veranstalter","varchar",150));
    event->addColumn(new DBColumn("int_edv","smallint",0,true,"0"));
    event->addColumn(new DBColumn("int_helfer","smallint",0,true,"0"));
    event->addColumn(new DBColumn("int_kampfrichter","smallint",0,true,"0"));
    event->addColumn(new DBColumn("var_meldung_website","varchar",200));
    event->addColumn(new DBColumn("var_verwendungszweck","varchar",150));
    event->addColumn(new DBColumn("rel_meldegeld","real",0,true,"0"));
    event->addColumn(new DBColumn("rel_nachmeldung","real",0,true,"0"));
    event->addColumn(new DBColumn("bol_faellig_nichtantritt","boolean",0,true,"'false'"));
    event->addColumn(new DBColumn("bol_ummeldung_moeglich","boolean",0,true,"'false'"));
    event->addColumn(new DBColumn("bol_nachmeldung_moeglich","boolean",0,true,"'false'"));
    event->addColumn(new DBColumn("txt_meldung_an","text"));
    event->addColumn(new DBColumn("txt_startberechtigung","text"));
    event->addColumn(new DBColumn("txt_teilnahmebedingungen","text"));
    event->addColumn(new DBColumn("txt_siegerauszeichnung","text"));
    event->addColumn(new DBColumn("txt_kampfrichter","text"));
    event->addColumn(new DBColumn("txt_hinweise","text"));
    event->addContraint(new DBConstraint("fky_ansprechpartner","tfx_personen","int_ansprechpartner","int_personenid","RESTRICT","RESTRICT"));
    event->addContraint(new DBConstraint("fky_hauptwettkampf","tfx_veranstaltungen","int_hauptwettkampf","int_veranstaltungenid","RESTRICT","RESTRICT"));
    event->addContraint(new DBConstraint("fky_kontenid","tfx_konten","int_kontenid","int_kontenid","RESTRICT","RESTRICT"));
    event->addContraint(new DBConstraint("fky_meldung_an","tfx_personen","int_meldung_an","int_personenid","RESTRICT","RESTRICT"));
    event->addContraint(new DBConstraint("fky_wettkampforteid","tfx_wettkampforte","int_wettkampforteid","int_wettkampforteid","RESTRICT","RESTRICT"));

    DBTable *competition = new DBTable("tfx_wettkaempfe");
    competition->addColumn(new DBColumn("int_wettkaempfeid","integer",0,false,"","",true));
    competition->addColumn(new DBColumn("int_veranstaltungenid","integer",0,false));
    competition->addColumn(new DBColumn("int_bereicheid","integer",0,false));
    competition->addColumn(new DBColumn("int_typ","smallint",0,true,"0"));
    competition->addColumn(new DBColumn("var_nummer","varchar",5));
    competition->addColumn(new DBColumn("var_name","varchar",150));
    competition->addColumn(new DBColumn("yer_von","smallint",0,false));
    competition->addColumn(new DBColumn("yer_bis","smallint"));
    competition->addColumn(new DBColumn("int_qualifikation","smallint",0,true,"0"));
    competition->addColumn(new DBColumn("int_wertungen","smallint"));
    competition->addColumn(new DBColumn("bol_streichwertung","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_ak_anzeigen","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_wahlwettkampf","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("int_durchgang","smallint",0,true,"1"));
    competition->addColumn(new DBColumn("int_bahn","smallint",0,true,"1"));
    competition->addColumn(new DBColumn("tim_startzeit","time without time zone"));
    competition->addColumn(new DBColumn("tim_einturnen","time without time zone"));
    competition->addColumn(new DBColumn("bol_info_anzeigen","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_kp","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_sortasc","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_mansort","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("bol_gerpkt","boolean",0,true,"'false'"));
    competition->addColumn(new DBColumn("int_anz_streich","smallint",0,true,"0"));
    competition->addContraint(new DBConstraint("fky_bereicheid","tfx_bereiche","int_bereicheid","int_bereicheid","RESTRICT","RESTRICT"));
    competition->addContraint(new DBConstraint("fky_veranstaltungenid","tfx_veranstaltungen","int_veranstaltungenid","int_veranstaltungenid","RESTRICT","CASCADE"));

    DBTable *competitionDiscipline = new DBTable("tfx_wettkaempfe_x_disziplinen");
    competitionDiscipline->addColumn(new DBColumn("int_wettkaempfe_x_disziplinenid","integer",0,false,"","",true));
    competitionDiscipline->addColumn(new DBColumn("int_wettkaempfeid","integer",0,false));
    competitionDiscipline->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    competitionDiscipline->addColumn(new DBColumn("var_ausschreibung","varchar",100));
    competitionDiscipline->addColumn(new DBColumn("int_sortierung","smallint"));
    competitionDiscipline->addColumn(new DBColumn("bol_kp","boolean",0,true,"'false'"));
    competitionDiscipline->addColumn(new DBColumn("rel_max","real",0,true,"0"));
    competitionDiscipline->addContraint(new DBConstraint("fky_disziplinenid","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","RESTRICT"));
    competitionDiscipline->addContraint(new DBConstraint("fky_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","int_wettkaempfeid","RESTRICT","CASCADE"));

    DBTable *competitionDisciplinPosition = new DBTable("tfx_wettkaempfe_dispos");
    competitionDisciplinPosition->addColumn(new DBColumn("int_wettkaempfe_disposid","integer",0,false,"","",true));
    competitionDisciplinPosition->addColumn(new DBColumn("int_wettkaempfe_x_disziplinenid","integer",0,false));
    competitionDisciplinPosition->addColumn(new DBColumn("int_sortx","smallint",0,true,"0"));
    competitionDisciplinPosition->addColumn(new DBColumn("int_sorty","smallint",0,true,"0"));
    competitionDisciplinPosition->addColumn(new DBColumn("int_kp","smallint",0,true,"0"));
    competitionDisciplinPosition->addContraint(new DBConstraint("fky_wettkaempfe_x_disziplinenid","tfx_wettkaempfe_x_disziplinen","int_wettkaempfe_x_disziplinenid","int_wettkaempfe_x_disziplinenid","RESTRICT","RESTRICT"));

    DBTable *team = new DBTable("tfx_mannschaften");
    team->addColumn(new DBColumn("int_mannschaftenid","integer",0,false,"","",true));
    team->addColumn(new DBColumn("int_wettkaempfeid","integer",0,false));
    team->addColumn(new DBColumn("int_vereineid","integer",0,false));
    team->addColumn(new DBColumn("int_nummer","smallint",0,true,"1"));
    team->addColumn(new DBColumn("var_riege","varchar",5,true,"1"));
    team->addColumn(new DBColumn("int_startnummer","integer"));
    team->addContraint(new DBConstraint("fky_vereineid","tfx_vereine","int_vereineid","int_vereineid","RESTRICT","RESTRICT"));
    team->addContraint(new DBConstraint("fky_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","int_wettkaempfeid","RESTRICT","CASCADE"));

    DBTable *penalty = new DBTable("tfx_mannschaften_abzug");
    penalty->addColumn(new DBColumn("int_mannschaften_abzugid","integer",0,false,"","",true));
    penalty->addColumn(new DBColumn("var_name","varchar",100));
    penalty->addColumn(new DBColumn("rel_abzug","real"));

    DBTable *teamPenalty = new DBTable("tfx_man_x_man_ab");
    teamPenalty->addColumn(new DBColumn("int_man_x_man_abid","integer",0,false,"","",true));
    teamPenalty->addColumn(new DBColumn("int_mannschaftenid","integer",0,false));
    teamPenalty->addColumn(new DBColumn("int_mannschaften_abzugid","integer",0,false));
    teamPenalty->addContraint(new DBConstraint("fky_mannschaften_abzugid","tfx_mannschaften_abzug","int_mannschaften_abzugid","int_mannschaften_abzugid","RESTRICT","RESTRICT"));
    teamPenalty->addContraint(new DBConstraint("fky_mannschaftenid","tfx_mannschaften","int_mannschaftenid","int_mannschaftenid","RESTRICT","CASCADE"));

    DBTable *teamMember = new DBTable("tfx_man_x_teilnehmer");
    teamMember->addColumn(new DBColumn("int_man_x_teilnehmerid","integer",0,false,"","",true));
    teamMember->addColumn(new DBColumn("int_mannschaftenid","integer",0,false));
    teamMember->addColumn(new DBColumn("int_teilnehmerid","integer",0,false));
    teamMember->addColumn(new DBColumn("int_runde","smallint"));
    teamMember->addContraint(new DBConstraint("fky_mannschaftenid","tfx_mannschaften","int_mannschaftenid","int_mannschaftenid","RESTRICT","CASCADE"));
    teamMember->addContraint(new DBConstraint("fky_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","int_teilnehmerid","RESTRICT","RESTRICT"));

    DBTable *groupMember = new DBTable("tfx_gruppen_x_teilnehmer");
    groupMember->addColumn(new DBColumn("int_gruppen_x_teilnehmerid","integer",0,false,"","",true));
    groupMember->addColumn(new DBColumn("int_gruppenid","integer",0,false));
    groupMember->addColumn(new DBColumn("int_teilnehmerid","integer",0,false));
    groupMember->addContraint(new DBConstraint("fky_gruppenid","tfx_gruppen","int_gruppenid","int_gruppenid","RESTRICT","CASCADE"));
    groupMember->addContraint(new DBConstraint("fky_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","int_teilnehmerid","RESTRICT","RESTRICT"));

    DBTable *squadDiscipline = new DBTable("tfx_riegen_x_disziplinen");
    squadDiscipline->addColumn(new DBColumn("int_riegen_x_disziplinenid","integer",0,false,"","",true));
    squadDiscipline->addColumn(new DBColumn("int_veranstaltungenid","integer",0,false));
    squadDiscipline->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    squadDiscipline->addColumn(new DBColumn("int_statusid","integer",0,false));
    squadDiscipline->addColumn(new DBColumn("var_riege","varchar",5));
    squadDiscipline->addColumn(new DBColumn("int_runde","smallint"));
    squadDiscipline->addColumn(new DBColumn("bol_erstes_geraet","boolean",0,true,"'false'"));
    squadDiscipline->addContraint(new DBConstraint("fky_disziplinenid","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","CASCADE"));
    squadDiscipline->addContraint(new DBConstraint("fky_statusid","tfx_status","int_statusid","int_statusid","RESTRICT","RESTRICT"));
    squadDiscipline->addContraint(new DBConstraint("fky_veranstaltungenid","tfx_veranstaltungen","int_veranstaltungenid","int_veranstaltungenid","RESTRICT","CASCADE"));

    DBTable *judgement = new DBTable("tfx_wertungen");
    judgement->addColumn(new DBColumn("int_wertungenid","integer",0,false,"","",true));
    judgement->addColumn(new DBColumn("int_wettkaempfeid","integer",0,false));
    judgement->addColumn(new DBColumn("int_teilnehmerid","integer"));
    judgement->addColumn(new DBColumn("int_gruppenid","integer"));
    judgement->addColumn(new DBColumn("int_mannschaftenid","integer"));
    judgement->addColumn(new DBColumn("int_statusid","integer",0,false));
    judgement->addColumn(new DBColumn("int_runde","smallint",0,true,"1"));
    judgement->addColumn(new DBColumn("int_startnummer","integer"));
    judgement->addColumn(new DBColumn("bol_ak","boolean",0,true,"'false'"));
    judgement->addColumn(new DBColumn("bol_startet_nicht","boolean",0,true,"'false'"));
    judgement->addColumn(new DBColumn("var_riege","varchar",5));
    judgement->addColumn(new DBColumn("var_comment","varchar",150));
    judgement->addContraint(new DBConstraint("fky_gruppenid","tfx_gruppen","int_gruppenid","int_gruppenid","RESTRICT","CASCADE"));
    judgement->addContraint(new DBConstraint("fky_mannschaftenid","tfx_mannschaften","int_mannschaftenid","int_mannschaftenid","RESTRICT","CASCADE"));
    judgement->addContraint(new DBConstraint("fky_statusid","tfx_status","int_statusid","int_statusid","RESTRICT","RESTRICT"));
    judgement->addContraint(new DBConstraint("fky_teilnehmerid","tfx_teilnehmer","int_teilnehmerid","int_teilnehmerid","RESTRICT","RESTRICT"));
    judgement->addContraint(new DBConstraint("fky_wettkaempfeid","tfx_wettkaempfe","int_wettkaempfeid","int_wettkaempfeid","RESTRICT","CASCADE"));

    DBTable *judgementDetail = new DBTable("tfx_wertungen_details");
    judgementDetail->addColumn(new DBColumn("int_wertungen_detailsid","integer",0,false,"","",true));
    judgementDetail->addColumn(new DBColumn("int_wertungenid","integer",0,false));
    judgementDetail->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    judgementDetail->addColumn(new DBColumn("int_versuch","smallint"));
    judgementDetail->addColumn(new DBColumn("rel_leistung","real"));
    judgementDetail->addColumn(new DBColumn("int_kp","smallint",0,true,"0"));
    judgementDetail->addContraint(new DBConstraint("fky_disziplinenid","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","RESTRICT"));
    judgementDetail->addContraint(new DBConstraint("fky_wertungenid","tfx_wertungen","int_wertungenid","int_wertungenid","RESTRICT","CASCADE"));

    DBTable *judgementDisciplin = new DBTable("tfx_wertungen_x_disziplinen");
    judgementDisciplin->addColumn(new DBColumn("int_wertungen_x_disziplinenid","integer",0,false,"","",true));
    judgementDisciplin->addColumn(new DBColumn("int_wertungenid","integer",0,false));
    judgementDisciplin->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    judgementDisciplin->addContraint(new DBConstraint("fky_disziplinenid","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","RESTRICT"));
    judgementDisciplin->addContraint(new DBConstraint("fky_wertungenid","tfx_wertungen","int_wertungenid","int_wertungenid","RESTRICT","CASCADE"));

    DBTable *juryJudgement = new DBTable("tfx_jury_results");
    juryJudgement->addColumn(new DBColumn("int_juryresultsid","integer",0,false,"","",true));
    juryJudgement->addColumn(new DBColumn("int_wertungenid","integer",0,false));
    juryJudgement->addColumn(new DBColumn("int_disziplinen_felderid","integer",0,false));
    juryJudgement->addColumn(new DBColumn("int_versuch","smallint"));
    juryJudgement->addColumn(new DBColumn("rel_leistung","real"));
    juryJudgement->addColumn(new DBColumn("int_kp","smallint"));
    juryJudgement->addContraint(new DBConstraint("fky_disziplinen_felderid","tfx_disziplinen_felder","int_disziplinen_felderid","int_disziplinen_felderid","RESTRICT","RESTRICT"));
    juryJudgement->addContraint(new DBConstraint("fky_wertungenid","tfx_wertungen","int_wertungenid","int_wertungenid","RESTRICT","CASCADE"));

    DBTable *layout = new DBTable("tfx_layouts");
    layout->addColumn(new DBColumn("int_layoutid","integer",0,false,"","",true));
    layout->addColumn(new DBColumn("var_name","varchar",100));
    layout->addColumn(new DBColumn("txt_comment","text"));

    DBTable *layoutField = new DBTable("tfx_layout_felder");
    layoutField->addColumn(new DBColumn("int_layout_felderid","integer",0,false,"","",true));
    layoutField->addColumn(new DBColumn("int_layoutid","integer",0,false));
    layoutField->addColumn(new DBColumn("int_typ","smallint"));
    layoutField->addColumn(new DBColumn("var_font","varchar",150));
    layoutField->addColumn(new DBColumn("rel_x","real"));
    layoutField->addColumn(new DBColumn("rel_y","real"));
    layoutField->addColumn(new DBColumn("rel_w","real"));
    layoutField->addColumn(new DBColumn("rel_h","real"));
    layoutField->addColumn(new DBColumn("var_value","varchar",200));
    layoutField->addColumn(new DBColumn("int_align","smallint",0,true,"0"));
    layoutField->addColumn(new DBColumn("int_layer","smallint"));
    layoutField->addContraint(new DBConstraint("fky_layoutid","tfx_layouts","int_layoutid","int_layoutid","RESTRICT","CASCADE"));

    DBTable *disciplinGroup = new DBTable("tfx_disziplinen_gruppen");
    disciplinGroup->addColumn(new DBColumn("int_disziplinen_gruppenid","integer",0,false,"","",true));
    disciplinGroup->addColumn(new DBColumn("var_name","varchar",100));
    disciplinGroup->addColumn(new DBColumn("txt_comment","text"));

    DBTable *disciplinGroupItem = new DBTable("tfx_disgrp_x_disziplinen");
    disciplinGroupItem->addColumn(new DBColumn("int_disgrp_x_disziplinenid","integer",0,false,"","",true));
    disciplinGroupItem->addColumn(new DBColumn("int_disziplinen_gruppenid","integer",0,false));
    disciplinGroupItem->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    disciplinGroupItem->addColumn(new DBColumn("int_pos","smallint"));
    disciplinGroupItem->addContraint(new DBConstraint("fky_disziplinen_gruppenid","tfx_disziplinen_gruppen","int_disziplinen_gruppenid","int_disziplinen_gruppenid","RESTRICT","RESTRICT"));
    disciplinGroupItem->addContraint(new DBConstraint("fky_disziplinen","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","CASCADE"));

    DBTable *startingOrder = new DBTable("tfx_startreihenfolge");
    startingOrder->addColumn(new DBColumn("int_startreihenfolgeid","integer",0,false,"","",true));
    startingOrder->addColumn(new DBColumn("int_wertungenid","integer",0,false));
    startingOrder->addColumn(new DBColumn("int_disziplinenid","integer",0,false));
    startingOrder->addColumn(new DBColumn("int_pos","smallint"));
    startingOrder->addColumn(new DBColumn("int_kp","smallint",0,true,"0"));
    startingOrder->addContraint(new DBConstraint("fky_wertungenid","tfx_wertungen","int_wertungenid","int_wertungenid","RESTRICT","RESTRICT"));
    startingOrder->addContraint(new DBConstraint("fky_disziplinen","tfx_disziplinen","int_disziplinenid","int_disziplinenid","RESTRICT","CASCADE"));

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
}

void DBTable::clearTables()
{
    tables.clear();
}

int DBTable::tableCount()
{
    return tables.count();
}
