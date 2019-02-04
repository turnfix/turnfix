#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QInputDialog>
#include <QFileDialog>
#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStandardItemModel>
#include "win_dbase.h"
#include <QSettings>
#include <QSqlError>

QString datname;
QStringList versions = QStringList() << "0.9.0" << "0.9.1" << "0.9.2";
QStringList tables = QStringList() << "disziplinen" << "disziplinfelder" << "mabzug" << "manab" << "mannschaften" << "mantn" << "meldeleistungen" << "rgdis" << "status" << "sub_wettkaempfe" << "teilnehmer" << "tndis" << "vereine" << "wertungen" << "wertungendetails" << "wettkaempfe" << "wkdis";
int order[17] = {12,10,2,15,9,8,13,4,5,3,0,11,7,16,6,14,1};
QStringList table_data[17][3] = {
    {
        QStringList() << "disziplinid" << "integer" << "disziplinname" << "text" << "disziplinformel" << "text" << "disziplinmaske" << "text" << "disziplinbereich" << "character" << "disziplinsport" << "character" << "disziplinversuche" << "smallint" << "disziplinicon" << "text" << "disziplinshortcut" << "text" << "disziplincalc" << "smallint" << "disziplineinheit" << "text" << "disziplinshort" << "text" << "disziplinshort2" << "text" << "disziplinbahnen" << "smallint",
        QStringList() << "disziplinid" << "integer" << "disziplinname" << "text" << "disziplinformel" << "text" << "disziplinmaske" << "text" << "disziplinbereich" << "character" << "disziplinsport" << "character" << "disziplinversuche" << "smallint" << "disziplinicon" << "text" << "disziplinshortcut" << "text" << "disziplincalc" << "smallint" << "disziplineinheit" << "text" << "disziplinshort" << "text" << "disziplinshort2" << "text" << "disziplinbahnen" << "smallint",
        QStringList() << "disziplinid" << "integer" << "disziplinname" << "text" << "disziplinformel" << "text" << "disziplinmaske" << "text" << "disziplinbereich" << "character" << "disziplinsport" << "character" << "disziplinversuche" << "smallint" << "disziplinicon" << "text" << "disziplinshortcut" << "text" << "disziplincalc" << "smallint" << "disziplineinheit" << "text" << "disziplinshort" << "text" << "disziplinshort2" << "text" << "disziplinbahnen" << "smallint"
    },
{
        QStringList() << "disziplinfeldid" << "integer" << "disziplinid" << "smallint" << "disziplinfeldbez" << "text",
        QStringList() << "disziplinfeldid" << "integer" << "disziplinid" << "smallint" << "disziplinfeldbez" << "text",
        QStringList() << "disziplinfeldid" << "integer" << "disziplinid" << "smallint" << "disziplinfeldbez" << "text" << "sort" << "smallint"
    },
{
        QStringList() << "mabzugid" << "integer" << "mabbez" << "text" << "mabpkt" << "real",
        QStringList() << "mabzugid" << "integer" << "mabbez" << "text" << "mabpkt" << "real",
        QStringList() << "mabzugid" << "integer" << "mabbez" << "text" << "mabpkt" << "real"
    },
{
        QStringList() << "manabid" << "integer" << "mabzugid" << "smallint" << "mannschaftsid" << "smallint",
        QStringList() << "manabid" << "integer" << "mabzugid" << "smallint" << "mannschaftsid" << "smallint",
        QStringList() << "manabid" << "integer" << "mabzugid" << "smallint" << "mannschaftsid" << "smallint"
    },
{
        QStringList() << "mannschaftsid" << "integer" << "vereinid" << "smallint" << "mannschaftsnr" << "smallint" << "sub_wettkampfid" << "smallint" << "riege" << "smallint",
        QStringList() << "mannschaftsid" << "integer" << "vereinid" << "smallint" << "mannschaftsnr" << "smallint" << "sub_wettkampfid" << "smallint" << "riege" << "smallint",
        QStringList() << "mannschaftsid" << "integer" << "vereinid" << "smallint" << "mannschaftsnr" << "smallint" << "sub_wettkampfid" << "bigint" << "riege" << "smallint" << "startnummer" << "bigint"
    },
{
        QStringList() << "mantnid" << "integer" << "mannschaftsid" << "smallint" << "teilnehmerid" << "smallint",
        QStringList() << "mantnid" << "integer" << "mannschaftsid" << "smallint" << "teilnehmerid" << "smallint" << "runde" << "smallint",
        QStringList() << "mantnid" << "integer" << "mannschaftsid" << "smallint" << "teilnehmerid" << "smallint" << "runde" << "smallint"
    },
{
        QStringList() << "meldeid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "meldeleistung" << "real",
        QStringList() << "meldeid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "meldeleistung" << "real",
        QStringList() << "meldeid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "meldeleistung" << "real"
    },
{
        QStringList(),
        QStringList(),
        QStringList() << "rgdisid" << "integer" << "wettkampfid" << "bigint" << "disziplinid" << "smallint" << "riege" << "smallint" << "firstdis" << "boolean" << "runde" << "smallint" << "statusid" << "smallint"
    },
{
        QStringList(),
        QStringList(),
        QStringList() << "statusid" << "integer" << "statusname" << "text" << "colorcode" << "ARRAY" << "bogen" << "boolean" << "karte" << "boolean"
    },
{
        QStringList() << "sub_wettkampfid" << "integer" << "sub_wettkampfnr" << "smallint" << "sub_wettkampfname" << "text" << "sub_wettkampfj1" << "smallint" << "sub_wettkampfj2" << "smallint" << "wettkampfid" << "bigint" << "quali" << "smallint" << "geschlecht" << "character",
        QStringList() << "sub_wettkampfid" << "integer" << "sub_wettkampfnr" << "smallint" << "sub_wettkampfname" << "text" << "sub_wettkampfj1" << "smallint" << "sub_wettkampfj2" << "smallint" << "wettkampfid" << "bigint" << "quali" << "smallint" << "geschlecht" << "character" << "streichmin" << "character" << "jganzeige" << "character" << "durchgang" << "smallint" << "startzeit" << "time without time zone",
        QStringList() << "sub_wettkampfid" << "integer" << "sub_wettkampfnr" << "bigint" << "sub_wettkampfname" << "text" << "sub_wettkampfj1" << "smallint" << "sub_wettkampfj2" << "smallint" << "wettkampfid" << "bigint" << "quali" << "smallint" << "geschlecht" << "character" << "streichmin" << "character" << "jganzeige" << "character" << "durchgang" << "smallint" << "startzeit" << "time without time zone" << "wahlwk" << "character" << "einturnen" << "time without time zone" << "bahn" << "smallint" << "showinfo" << "boolean"
    },
{
        QStringList() << "teilnehmerid" << "integer" << "vereinid" << "bigint" << "tnname" << "text" << "geschlecht" << "character" << "geburtstag" << "smallint",
        QStringList() << "teilnehmerid" << "integer" << "vereinid" << "bigint" << "tnname" << "text" << "geschlecht" << "character" << "geburtstag" << "smallint",
        QStringList() << "teilnehmerid" << "integer" << "vereinid" << "bigint" << "tnname" << "text" << "geschlecht" << "character" << "geburtstag" << "smallint" << "startpass" << "bigint"
    },
{
        QStringList(),
        QStringList(),
        QStringList() << "tndisid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint"
    },
{
        QStringList() << "vereinid" << "integer" << "vereinname" << "text",
        QStringList() << "vereinid" << "integer" << "vereinname" << "text" << "ansprech" << "text" << "adresse" << "text" << "plz" << "text" << "ort" << "text" << "tel" << "text" << "fax" << "text" << "email" << "text" << "website" << "text",
        QStringList() << "vereinid" << "integer" << "vereinname" << "text" << "ansprech" << "text" << "adresse" << "text" << "plz" << "text" << "ort" << "text" << "tel" << "text" << "fax" << "text" << "email" << "text" << "website" << "text" << "startort" << "smallint"
    },
{
        QStringList() << "wertungsid" << "integer" << "teilnehmerid" << "bigint" << "sub_wettkampfid" << "bigint" << "riege" << "smallint" << "ak" << "smallint" << "mannschaftsid" <<"smallint",
        QStringList() << "wertungsid" << "integer" << "teilnehmerid" << "bigint" << "sub_wettkampfid" << "bigint" << "riege" << "smallint" << "ak" << "smallint" << "mannschaftsid" <<"smallint" << "runde" << "smallint",
        QStringList() << "wertungsid" << "integer" << "teilnehmerid" << "bigint" << "sub_wettkampfid" << "bigint" << "riege" << "smallint" << "ak" << "smallint" << "mannschaftsid" <<"smallint" << "runde" << "smallint" << "startnummer" << "bigint" << "nostart" << "boolean" << "statusid" << "smallint"
    },
{
        QStringList() << "detailsid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "disziplinversuch" << "smallint" << "detailswertung" << "real",
        QStringList() << "detailsid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "disziplinversuch" << "smallint" << "detailswertung" << "real",
        QStringList() << "detailsid" << "integer" << "wertungsid" << "smallint" << "disziplinid" << "smallint" << "disziplinversuch" << "smallint" << "detailswertung" << "real"
    },
{
        QStringList() << "wettkampfid" << "integer" << "wettkampfname" << "text" << "wettkampfdatum" << "date" << "wettkampfort" << "text" << "wettkampftyp" << "character" << "wettkampfveranstalter" << "text" << "wettkampfwertung" << "smallint",
        QStringList() << "wettkampfid" << "integer" << "wettkampfname" << "text" << "wettkampfdatum" << "date" << "wettkampfort" << "text" << "wettkampftyp" << "character" << "wettkampfveranstalter" << "text" << "wettkampfwertung" << "smallint" << "wettkampfbis" << "date" << "meldeschluss" << "date" << "strasse" << "text" << "ort" << "text" << "edv" << "smallint" << "kari" << "smallint" << "helfer" << "smallint" << "kname" << "text" << "kstrasse" << "text" << "kort" << "text" << "ktel" << "text" << "kfax" << "text" << "kemail" << "text" << "kweb" << "text" << "mname" << "text" << "mstrasse" << "text" << "mort" << "text" << "mtel" << "text" << "mfax" << "text" << "memail" << "text" << "kplz" << "text" << "mplz" << "text" << "plz" << "text" << "meldegeld" << "real" << "nichtantritt" << "character" << "ummeldung" << "character" << "nachmeldung" << "character" << "zmeldegeld" << "real" << "zmeldung" << "text" << "startb" << "text" << "teilnahme" << "text" << "sieger" << "text" << "akari" << "text" << "hinweise" << "text" << "kinhaber" << "text" << "knummer" << "text" << "kblz" << "text" << "kbank" << "text" << "kzweck" << "text" << "runde" << "smallint" << "hauptwettkampf" << "smallint" << "rwk" << "character",
        QStringList() << "wettkampfid" << "integer" << "wettkampfname" << "text" << "wettkampfdatum" << "date" << "wettkampfort" << "text" << "wettkampftyp" << "character" << "wettkampfveranstalter" << "text" << "wettkampfwertung" << "smallint" << "wettkampfbis" << "date" << "meldeschluss" << "date" << "strasse" << "text" << "ort" << "text" << "edv" << "smallint" << "kari" << "smallint" << "helfer" << "smallint" << "kname" << "text" << "kstrasse" << "text" << "kort" << "text" << "ktel" << "text" << "kfax" << "text" << "kemail" << "text" << "kweb" << "text" << "mname" << "text" << "mstrasse" << "text" << "mort" << "text" << "mtel" << "text" << "mfax" << "text" << "memail" << "text" << "kplz" << "text" << "mplz" << "text" << "plz" << "text" << "meldegeld" << "real" << "nichtantritt" << "character" << "ummeldung" << "character" << "nachmeldung" << "character" << "zmeldegeld" << "real" << "zmeldung" << "text" << "startb" << "text" << "teilnahme" << "text" << "sieger" << "text" << "akari" << "text" << "hinweise" << "text" << "kinhaber" << "text" << "knummer" << "text" << "kblz" << "text" << "kbank" << "text" << "kzweck" << "text" << "runde" << "smallint" << "hauptwettkampf" << "smallint" << "rwk" << "character"
    },
{
        QStringList() << "wkdisid" << "integer" << "sub_wettkampfid" << "smallint" << "disziplinid" << "smallint",
        QStringList() << "wkdisid" << "integer" << "sub_wettkampfid" << "smallint" << "disziplinid" << "smallint" << "aextra" << "text",
        QStringList() << "wkdisid" << "integer" << "sub_wettkampfid" << "bigint" << "disziplinid" << "smallint" << "aextra" << "text"
    }
};
QStringList constraint_data[17][3] = {
    { //disziplinen
        QStringList() << "disziplinid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "disziplinid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "disziplinid" << "PRIMARY KEY" << "" << "" << ""
    },
{ //disziplinfelder
        QStringList() << "disziplinfeldid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen",
        QStringList() << "disziplinfeldid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen",
        QStringList() << "disziplinfeldid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen"
    },
{ //mabzug
        QStringList() << "mabzugid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "mabzugid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "mabzugid" << "PRIMARY KEY" << "" << "" << ""
    },
{ //manab
        QStringList() << "manabid" << "PRIMARY KEY" << "" << "" << "" << "mabzugid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mabzug" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mannschaften",
        QStringList() << "manabid" << "PRIMARY KEY" << "" << "" << "" << "mabzugid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mabzug" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mannschaften",
        QStringList() << "manabid" << "PRIMARY KEY" << "" << "" << "" << "mabzugid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mabzug" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mannschaften"
    },
{ //mannschaften
        QStringList() << "mannschaftsid" << "PRIMARY KEY" << "" << "" << "" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe"<< "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine",
        QStringList() << "mannschaftsid" << "PRIMARY KEY" << "" << "" << "" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe"<< "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine",
        QStringList() << "mannschaftsid" << "PRIMARY KEY" << "" << "" << "" << "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe"
    },
{ //mantn
        QStringList() << "mantnid" << "PRIMARY KEY" << "" << "" << "" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mannschaften" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer",
        QStringList() << "mantnid" << "PRIMARY KEY" << "" << "" << "" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "mannschaften" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer",
        QStringList() << "mantnid" << "PRIMARY KEY" << "" << "" << "" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer" << "mannschaftsid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "mannschaften"
    },
{ //meldeleistungen
        QStringList() << "meldeid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wertungen",
        QStringList() << "meldeid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wertungen",
        QStringList() << "meldeid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "wertungen"
    },
{ //rgdis
        QStringList(),
        QStringList(),
        QStringList() << "rgdisid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wettkaempfe" << "statusid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "status"
    },
{ //status
        QStringList(),
        QStringList(),
        QStringList() << "statusid" << "PRIMARY KEY" << "" << "" << ""
    },
{ //sub_wettkaempfe
        QStringList() << "sub_wettkampfid" << "PRIMARY KEY" << "" << "" << "" << "wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wettkaempfe",
        QStringList() << "sub_wettkampfid" << "PRIMARY KEY" << "" << "" << "" << "wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wettkaempfe",
        QStringList() << "sub_wettkampfid" << "PRIMARY KEY" << "" << "" << "" << "wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wettkaempfe"
    },
{ //teilnehmer
        QStringList() << "teilnehmerid" << "PRIMARY KEY" << "" << "" << "" << "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine",
        QStringList() << "teilnehmerid" << "PRIMARY KEY" << "" << "" << "" << "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine",
        QStringList() << "teilnehmerid" << "PRIMARY KEY" << "" << "" << "" << "vereinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "vereine"
    },
{ //tndis
        QStringList(),
        QStringList(),
        QStringList() << "tndisid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "wertungen"
    },
{ //vereine
        QStringList() << "vereinid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "vereinid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "vereinid" << "PRIMARY KEY" << "" << "" << ""
    },
{ //wertungen
        QStringList() << "wertungsid" << "PRIMARY KEY" << "" << "" << "" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer",
        QStringList() << "wertungsid" << "PRIMARY KEY" << "" << "" << "" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer",
        QStringList() << "wertungsid" << "PRIMARY KEY" << "" << "" << "" << "teilnehmerid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "teilnehmer" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "sub_wettkaempfe" << "statusid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "status"
    },
{ //wertungendetails
        QStringList() << "detailsid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wertungen",
        QStringList() << "detailsid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "wertungen",
        QStringList() << "detailsid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "wertungsid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "wertungen"
    },
{ //wettkaempfe
        QStringList() << "wettkampfid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "wettkampfid" << "PRIMARY KEY" << "" << "" << "",
        QStringList() << "wettkampfid" << "PRIMARY KEY" << "" << "" << ""
    },
{ //wkdis
        QStringList() << "wkdisid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe",
        QStringList() << "wkdisid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "sub_wettkaempfe",
        QStringList() << "wkdisid" << "PRIMARY KEY" << "" << "" << "" << "disziplinid" << "FOREIGN KEY" << "NO ACTION" << "NO ACTION" << "disziplinen" << "sub_wettkampfid" << "FOREIGN KEY" << "NO ACTION" << "CASCADE" << "sub_wettkaempfe"
    }
};
QStringList functions = QStringList() << "calc" << "ergebnisse_new" << "mannschaftsergebnisse_new" << "startlist" << "rundenergebnisse_new" << "riege" << "wertung";
QString functions_data[7] = {
    " DECLARE f text; ret real; BEGIN SELECT INTO f replace(replace(disziplinformel,'x',$2::text),',','.') FROM disziplinen WHERE disziplinid=$1; EXECUTE 'SELECT round(' || f || ',' || $3 || ') FROM disziplinen WHERE disziplinid=' || $1 into ret; return ret; END; ",
    " DECLARE wknr ALIAS FOR $1; swknr ALIAS FOR $2; wkrec RECORD; pre numeric(10,3)=0; quali integer=0; lquali numeric(10,3)=0; iplatz integer=1; i integer=1; col_list text[] := '{}'; query text; r RECORD; q RECORD; streichq text=''; BEGIN FOR q IN EXECUTE 'SELECT quali, streichmin FROM sub_wettkaempfe WHERE sub_wettkampfnr=' || swknr || ' AND wettkampfid=' || wknr || ' LIMIT 1' LOOP quali = q.quali; IF q.streichmin = '1' THEN streichq='(SELECT CASE WHEN (SELECT COUNT(*) FROM (SELECT DISTINCT ON (disziplinid) disziplinid, detailsid FROM wertungendetails WHERE wertungsid=wertungen.wertungsid ORDER BY disziplinid, detailswertung DESC) AS count) = (SELECT COUNT(*) FROM wkdis INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ') THEN MIN(detailswertung) ELSE 0 END::numeric FROM wertungendetails WHERE (disziplinid,detailsid) IN (SELECT DISTINCT ON (disziplinid) disziplinid, detailsid FROM wertungendetails WHERE wertungsid=wertungen.wertungsid ORDER BY disziplinid, detailswertung DESC))'; ELSE streichq='0'; END IF; END LOOP; FOR r IN EXECUTE 'SELECT DISTINCT disziplinid, disziplinsport, disziplincalc, disziplinformel FROM disziplinen INNER JOIN wkdis USING (disziplinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' ORDER BY disziplinsport, disziplinid' LOOP col_list := array_append(col_list, 'round((' || replace(replace(r.disziplinformel,'x','max(case when disziplinid=' || r.disziplinid || ' then detailswertung else 0 end)'),',','.') || ')::numeric,' || r.disziplincalc || ')'); END LOOP; query := 'SELECT teilnehmerid::text AS platz, tnname, vereinname, substring(to_char(geburtstag,' || quote_literal('9999') || ') from 4 for 2), ' || array_to_string(col_list, ',') || ', ((' || array_to_string(col_list, '+') || ')-' || streichq || ')::numeric AS summe, wertungen.wertungsid FROM wertungendetails RIGHT JOIN wertungen USING (wertungsid) LEFT JOIN disziplinen USING (disziplinid) INNER JOIN teilnehmer USING (teilnehmerid) INNER JOIN vereine USING (vereinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' AND wertungen.nostart=false GROUP BY teilnehmerid, tnname, vereinname, geburtstag, wertungsid, ak ORDER BY wertungen.ak, summe DESC'; FOR wkrec IN EXECUTE query LOOP IF pre > wkrec.summe THEN iplatz = i; END IF; IF pre >= wkrec.summe OR i=1 THEN wkrec.platz = iplatz; IF (i-1)<quali THEN wkrec.platz = wkrec.platz || ' Q'; lquali=wkrec.summe; ELSE IF lquali = wkrec.summe AND quali>0 THEN wkrec.platz = wkrec.platz || ' Q'; END IF; END IF; ELSE wkrec.platz = 'AK'; wkrec.tnname = wkrec.tnname || ' (AK)'; END IF; pre = wkrec.summe; i=i+1; RETURN NEXT wkrec; END LOOP; END; ",
    " DECLARE wknr ALIAS FOR $1; swknr ALIAS FOR $2; cwertungen ALIAS FOR $3; runde ALIAS FOR $4; wkrec RECORD; abrec RECORD; pre numeric(10,3)=0; quali integer=0; lquali numeric(10,3)=0; abzug numeric(10,3)=0; iplatz integer=1; i integer=1; col_list text[] := '{}'; query text; r RECORD; q RECORD; BEGIN FOR q IN EXECUTE 'SELECT quali FROM sub_wettkaempfe WHERE sub_wettkampfnr=' || swknr || ' AND wettkampfid=' || wknr || ' LIMIT 1' LOOP quali = q.quali; END LOOP; FOR r IN EXECUTE 'SELECT DISTINCT disziplinid, disziplinsport, disziplincalc, disziplinname FROM disziplinen INNER JOIN wkdis USING (disziplinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' ORDER BY disziplinsport, disziplinid' LOOP col_list := array_append(col_list, '(SELECT wertung(' || quote_literal(wknr) || ',' || quote_literal(swknr) || ',' || quote_literal(cwertungen) || ',' || quote_literal(r.disziplincalc) || ',' || quote_literal(r.disziplinid) || ',mannschaften.mannschaftsid,' || runde || '))'); END LOOP; query := 'SELECT mannschaften.mannschaftsid::text AS platz, vereinname, mannschaften.mannschaftsnr ||' || quote_literal('. Mannschaft') || ',' || array_to_string(col_list, ',') || ', (' || array_to_string(col_list, '+') || ') AS summe, mannschaften.mannschaftsid FROM mannschaften INNER JOIN vereine ON vereine.vereinid = mannschaften.vereinid INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' GROUP BY vereine.vereinid, vereinname, mannschaften.mannschaftsid, mannschaftsnr ORDER BY summe DESC'; FOR wkrec IN EXECUTE query LOOP IF pre > wkrec.summe THEN iplatz = i; END IF; abzug=0; FOR abrec IN SELECT mabpkt::numeric AS tabzug FROM manab INNER JOIN mabzug ON manab.mabzugid = mabzug.mabzugid WHERE manab.mannschaftsid=wkrec.platz::integer LOOP abzug=abzug+abrec.tabzug; END LOOP; wkrec.summe = wkrec.summe-abzug; wkrec.platz = iplatz; IF (i-1)<quali THEN wkrec.platz = wkrec.platz || ' Q'; lquali=wkrec.summe; ELSE IF lquali = wkrec.summe AND quali>0 THEN wkrec.platz = wkrec.platz || ' Q'; END IF; END IF; pre = wkrec.summe; i=i+1; RETURN NEXT wkrec; END LOOP; END; ",
    " DECLARE wkid ALIAS FOR $1; rid ALIAS FOR $2; disid ALIAS FOR $3; bahnen ALIAS FOR $4; wrunde ALIAS FOR $5; bahnenn numeric(10,0); curr numeric(10,0); wkrec RECORD; rtemp startlist; lauf integer=1; bahn numeric(10,0)=1; opp integer=0; count integer=0; bahne integer[] := '{}'; bahnen2 numeric(10,0); BEGIN FOR wkrec IN SELECT teilnehmerid, tnname, vereinname, CASE WHEN EXISTS(SELECT meldeleistung FROM meldeleistungen WHERE wertungsid=wertungen.wertungsid AND disziplinid=wkdis.disziplinid) THEN (SELECT meldeleistung FROM meldeleistungen WHERE wertungsid=wertungen.wertungsid AND disziplinid=wkdis.disziplinid) ELSE 0 END AS leistung FROM wertungen INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) INNER JOIN wkdis USING (sub_wettkampfid) INNER JOIN teilnehmer USING (teilnehmerid) INNER JOIN vereine USING (vereinid) WHERE wettkampfid=wkid AND riege=rid AND disziplinid=disid AND runde=wrunde ORDER BY leistung DESC LOOP count = count+1; END LOOP; bahnenn = bahnen; IF round(bahnenn/2) = round(bahnenn/2,2) THEN curr = bahnen/2+1; ELSE curr = round(bahnenn/2,0); END IF; FOR i IN 1..bahnen LOOP bahne := array_append(bahne,curr::integer); IF opp = 0 THEN curr = curr - 1*i; opp = 1; ELSE curr = curr + 1*i; opp = 0; END IF; END LOOP; curr = 1; bahnen2 = ceil(count/ceil(count/bahnenn)); FOR wkrec IN SELECT startnummer, tnname, vereinname, CASE WHEN EXISTS(SELECT meldeleistung FROM meldeleistungen WHERE wertungsid=wertungen.wertungsid AND disziplinid=wkdis.disziplinid) THEN (SELECT meldeleistung FROM meldeleistungen WHERE wertungsid=wertungen.wertungsid AND disziplinid=wkdis.disziplinid) ELSE 0 END AS leistung FROM wertungen INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) INNER JOIN wkdis USING (sub_wettkampfid) INNER JOIN teilnehmer USING (teilnehmerid) INNER JOIN vereine USING (vereinid) WHERE wettkampfid=wkid AND riege=rid AND disziplinid=disid AND runde=wrunde AND  (NOT EXISTS (SELECT tndisid FROM tndis WHERE wertungsid=wertungen.wertungsid) OR EXISTS (SELECT tndisid FROM tndis WHERE tndis.wertungsid=wertungen.wertungsid AND tndis.disziplinid=disid)) ORDER BY leistung DESC LOOP rtemp = (lauf,bahne[bahn],wkrec.startnummer, wkrec.tnname, wkrec.vereinname); RETURN NEXT rtemp; IF bahn = bahnen2 THEN lauf=lauf+1; IF ceil((count-curr)/bahnenn) > 0 THEN bahnen2 = ceil((count-curr)/ceil((count-curr)/bahnenn)); END IF; bahn = 0; END IF; bahn = bahn + 1; curr = curr + 1; END LOOP; END; ",
    " DECLARE wknr ALIAS FOR $1; swknr ALIAS FOR $2; cwertungen ALIAS FOR $3; wkrec RECORD; pre numeric(10,3)=0; quali integer=0; lquali numeric(10,3)=0; iplatz integer=1; i integer=1; col_list text[] := '{}'; col_list2 text[] := '{}'; query text; r RECORD; d RECORD; q RECORD; BEGIN FOR q IN EXECUTE 'SELECT quali FROM sub_wettkaempfe WHERE sub_wettkampfnr=' || swknr || ' AND wettkampfid=' || wknr || ' LIMIT 1' LOOP quali = q.quali; END LOOP; FOR r IN EXECUTE 'SELECT runde FROM wettkaempfe WHERE wettkampfid=' || wknr || ' OR hauptwettkampf=' || wknr || ' ORDER BY runde' LOOP FOR d IN EXECUTE 'SELECT disziplinname FROM disziplinen INNER JOIN wkdis USING (disziplinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' ORDER BY disziplinsport, disziplinid' LOOP col_list2 := array_append(col_list2, '\"' || d.disziplinname || r.runde || '\"' || ' numeric'); END LOOP; col_list := array_append(col_list, '(SELECT summe FROM mannschaftsergebnisse_new(' || wknr || ',' || swknr || ',' || cwertungen || ',' || r.runde || ') AS (platz text, verein text, mannschaft text,' || array_to_string(col_list2, ',') || ', summe numeric,id integer) WHERE id=mannschaften.mannschaftsid)'); col_list2 := '{}'; END LOOP; query := 'SELECT mannschaften.mannschaftsid, mannschaften.mannschaftsid::text AS platz, vereinname, mannschaften.mannschaftsnr ||' || quote_literal('. Mannschaft') || ',' || array_to_string(col_list, '+') || ' AS summe,' || array_to_string(col_list, ',') || ' FROM mannschaften INNER JOIN vereine ON vereine.vereinid = mannschaften.vereinid INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND sub_wettkampfnr=' || swknr || ' GROUP BY vereine.vereinid, vereinname, mannschaften.mannschaftsid, mannschaftsnr ORDER BY summe DESC'; FOR wkrec IN EXECUTE query LOOP IF pre > wkrec.summe THEN iplatz = i; END IF; wkrec.platz = iplatz; IF (i-1)<quali THEN wkrec.platz = wkrec.platz || ' Q'; lquali=wkrec.summe; ELSE IF lquali = wkrec.summe AND quali>0 THEN wkrec.platz = wkrec.platz || ' Q'; END IF;  END IF; pre = wkrec.summe; i=i+1; RETURN NEXT wkrec; END LOOP; END; ",
    " DECLARE wknr ALIAS FOR $1; riege ALIAS FOR $2; runde ALIAS FOR $3; wkrec RECORD; col_list text[] := '{}'; query text; r RECORD; BEGIN FOR r IN EXECUTE 'SELECT disziplinid, disziplinname, disziplinversuche, disziplinmaske, position(' || quote_literal(':') || ' in disziplinmaske) FROM disziplinen INNER JOIN wkdis USING (disziplinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) INNER JOIN wertungen USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND riege=' || riege || ' GROUP BY disziplinid, disziplinname, disziplinsport, disziplinversuche, disziplinmaske ORDER BY disziplinsport, disziplinid' LOOP FOR i IN 1..r.disziplinversuche LOOP IF r.position != 0 THEN col_list := array_append(col_list,'lpad(floor(sum(case when disziplinid=' || quote_literal(r.disziplinid) || ' and disziplinversuch=' || i || ' then detailswertung else 0 end)/60)::text,2, ' || quote_literal('0') || ') || ' || quote_literal(':') || ' || overlay(lpad((sum(case when disziplinid=' || quote_literal(r.disziplinid) || ' and disziplinversuch=' || i || ' then detailswertung else 0 end)-(floor(sum(case when disziplinid=' || quote_literal(r.disziplinid) || ' and disziplinversuch=1 then detailswertung else 0 end)/60)::real*60::real))::text,5, ' || quote_literal('0') || ') placing ' || quote_literal('.') || ' from 3 for 1)'); ELSE col_list := array_append(col_list, 'to_char(sum(case when disziplinid=' || quote_literal(r.disziplinid) || ' and disziplinversuch=' || i || ' then detailswertung else 0 end),' || quote_literal(r.disziplinmaske) || ') as sprung1'); END IF; END LOOP; END LOOP; query := 'SELECT startnummer, tnname, vereinname, sub_wettkampfnr, ' || array_to_string(col_list, ',') || ', wertungsid FROM wertungendetails RIGHT JOIN wertungen USING (wertungsid) INNER JOIN teilnehmer USING (teilnehmerid) INNER JOIN vereine USING (vereinid) INNER JOIN sub_wettkaempfe USING (sub_wettkampfid) WHERE wettkampfid=' || wknr || ' AND riege=' || riege || ' AND runde=' || runde || ' AND nostart=false GROUP BY startnummer, tnname, vereinname, sub_wettkampfnr, wertungsid, mannschaftsid ORDER BY sub_wettkampfnr, startnummer'; FOR wkrec IN EXECUTE query LOOP RETURN NEXT wkrec; END LOOP; END; ",
    " DECLARE wknr ALIAS FOR $1; swknr ALIAS FOR $2; cwertungen ALIAS FOR $3; calc ALIAS FOR $4; disziplinid ALIAS FOR $5; mannschaftsid ALIAS FOR $6; runde ALIAS FOR $7; result numeric(10,3)=0; r RECORD; BEGIN FOR r IN EXECUTE 'SELECT COALESCE((SELECT calc(disziplinid,detailswertung,' || quote_literal(calc) || ') FROM wertungendetails WHERE disziplinid=' || quote_literal(disziplinid) || ' AND wertungendetails.wertungsid=wertungen.wertungsid ORDER BY detailswertung DESC LIMIT 1),0.000) AS wertung FROM wertungen WHERE mannschaftsid=' || quote_literal(mannschaftsid) || ' AND wertungen.ak=0 AND wertungen.runde=' || runde || ' ORDER BY wertung DESC LIMIT 3' LOOP result = result+r.wertung; END LOOP; RETURN result; END; "
};

DBCheck::DBCheck() {
    setupUi(this);
    tb_model = new QStandardItemModel();
    tb_model->setColumnCount(2);
    tb_table->setModel(tb_model);
    ft_model = new QStandardItemModel();
    ft_model->setColumnCount(2);
    ft_table->setModel(ft_model);
    QHeaderView::ResizeMode resizeMode[] = {QHeaderView::Stretch, QHeaderView::Fixed};
    int resize[] = {200,45};
    QString headers[2] = {"Name","Version"};
    for (int i=0;i<2;i++) {
        tb_model->setHeaderData(i, Qt::Horizontal, headers[i]);
        tb_table->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        tb_table->horizontalHeader()->resizeSection(i, resize[i]);
        ft_model->setHeaderData(i, Qt::Horizontal, headers[i]);
        ft_table->horizontalHeader()->setResizeMode(i, resizeMode[i]);
        ft_table->horizontalHeader()->resizeSection(i, resize[i]);
    }
    for (int i=0; i<tables.count();i++) {
        tb_model->setItem(i,0,new QStandardItem(tables.at(i)));
    }
    for (int i=0; i<functions.count();i++) {
        ft_model->setItem(i,0,new QStandardItem(functions.at(i)));
    }

    connect(but_backup, SIGNAL(clicked()), this, SLOT(backup()));
    connect(but_restore, SIGNAL(clicked()), this, SLOT(restore()));
    connect(but_updateTables, SIGNAL(clicked()), this, SLOT(updateTables()));
    connect(but_updateFunctions, SIGNAL(clicked()), this, SLOT(updateFunctions()));
    connect(but_install, SIGNAL(clicked()), this, SLOT(install()));
}

bool DBCheck::startConnect() {
    if (connectdb()) {
        check_versions();
    } else {
        QMessageBox msg(QMessageBox::Critical, "Fehler!", statusbar->currentMessage());
        msg.exec();
        return false;
    }
    return true;
}

void DBCheck::check_versions() {
    for (int t=0; t<tables.size(); t++) {
        QSqlQuery query;
        QStringList checklist;
        query.prepare("SELECT COUNT(*) FROM information_schema.tables WHERE table_schema ='public' AND table_name=?");
        query.bindValue(0,tables[t]);
        query.exec();
        query.next();
        if (query.value(0).toInt() > 0) {
            query.prepare("SELECT column_name, data_type FROM information_schema.columns WHERE table_name=? ORDER BY ordinal_position");
            query.bindValue(0,tables[t]);
            query.exec();
            while (query.next()) {
                checklist << query.value(0).toString() << query.value(1).toString();
            }
            bool check = false;
            for (int i=(versions.size()-1);i>=0;i--) {
                if (checklist == table_data[t][i]) {
                    QSqlQuery query2;
                    QStringList checklist2;
                    query2.prepare("SELECT c.conname AS constraint_name, CASE c.contype WHEN 'c' THEN 'CHECK' WHEN 'f' THEN 'FOREIGN KEY' WHEN 'p' THEN 'PRIMARY KEY' WHEN 'u' THEN 'UNIQUE' END AS \"constraint_type\", CASE confupdtype WHEN 'a' THEN 'NO ACTION' WHEN 'r' THEN 'RESTRICT' WHEN 'c' THEN 'CASCADE' WHEN 'n' THEN 'SET NULL' WHEN 'd' THEN 'SET DEFAULT' END AS on_update, CASE confdeltype WHEN 'a' THEN 'NO ACTION' WHEN 'r' THEN 'RESTRICT' WHEN 'c' THEN 'CASCADE' WHEN 'n' THEN 'SET NULL' WHEN 'd' THEN 'SET DEFAULT' END AS on_delete, t2.relname AS references_table FROM pg_constraint c LEFT JOIN pg_class t ON c.conrelid = t.oid LEFT JOIN pg_class t2 ON c.confrelid = t2.oid WHERE t.relname = ?");
                    query2.bindValue(0,tables[t]);
                    query2.exec();
                    while (query2.next()) {
                        checklist2 << query2.value(0).toString() << query2.value(1).toString() << query2.value(2).toString() << query2.value(3).toString() << query2.value(4).toString();
                    }
                    if (checklist2 == constraint_data[t][i]) {
                        tb_model->setItem(t,1,new QStandardItem(versions.at(i)));
                        check = true;
                        break;
                    }
                }
            }
            if (!check) {
                tb_model->setItem(t,1,new QStandardItem("fehlerhaft"));
            }
        } else {
            tb_model->setItem(t,1,new QStandardItem("existiert nicht"));
        }
    }
    for (int f=0; f<7; f++) {
        QSqlQuery query;
        query.prepare("SELECT routine_definition FROM information_schema.routines WHERE specific_schema NOT IN ('pg_catalog', 'information_schema') AND type_udt_name != 'trigger' AND routine_name=?");
        query.bindValue(0,functions[f]);
        query.exec();
        query.next();
        if (query.value(0).toString() == functions_data[f]) {
            ft_model->setItem(f,1,new QStandardItem("aktuell"));
        } else {
            ft_model->setItem(f,1,new QStandardItem("veraltet"));
        }
    }
}

bool DBCheck::connectdb() {
    QSettings settings("turnfix.ini", QSettings::IniFormat);
    settings.setIniCodec("ISO 8859-1");
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    if (settings.value("Database/Server").toString().length() == 0) {
        QString server = QInputDialog::getText(this, tr("Serveradresse"), tr("Geben sie die IP-Adresse oder den Namen des SQL-Servers ein!"),QLineEdit::Normal,"localhost");
        settings.setValue("Database/Server",server);
    }
    statusbar->showMessage("Verbindung zu " + settings.value("Database/Server").toString() + " wird hergestellt...");
    db.setHostName(settings.value("Database/Server").toString());
    db.setDatabaseName("postgres");
    if (settings.value("Database/Superuser").toString().length() == 0) {
        QString username = QInputDialog::getText(this, tr("Superuser"), tr("Geben sie den Benutzernamen des Superusers ein!"),QLineEdit::Normal,"postgres");
        settings.setValue("Database/Superuser",username);
    }
    QString password = QInputDialog::getText(this, tr("Passwort"), "Geben sie das Passwort von " + settings.value("Database/Superuser").toString() + " ein!",QLineEdit::Password);
    db.setUserName(settings.value("Database/Superuser").toString());
    db.setPassword(password);
    if(db.open()) {
        statusbar->showMessage("Erfolgreich!");
        statusbar->showMessage("Überprüfen ob der User ein Superuser ist und Datenbanken erstellen kann...");
        QSqlQuery query;
        query.prepare("SELECT usesuper, usecreatedb FROM pg_user WHERE usename=? LIMIT 1");
        query.bindValue(0,settings.value("Database/Superuser").toString());
        query.exec();
        query.next();
        if (query.value(0).toBool() == true && query.value(1).toBool() == true) {
            statusbar->showMessage("Erfolgreich!");
            statusbar->showMessage("Datenbank überprüfen...");
            if (settings.value("Database/Database").toString().length() == 0) {
                statusbar->showMessage("Noch kein Datenbankname angegeben...");
                QString username = QInputDialog::getText(this, tr("Datenbank"), tr("Geben sie Namen der Datenbank ein in der TurnFix die Daten speichern soll!"),QLineEdit::Normal,"turnfix");
                settings.setValue("Database/Database",username);
            }
            query.prepare("SELECT datname FROM pg_database WHERE datname=? LIMIT 1");
            query.bindValue(0,settings.value("Database/Database").toString());
            query.exec();
            int cont = 0;
            if (query.size() == 0) {
                statusbar->showMessage("Datenbank noch nicht vorhanden... Datenbank wird angelegt ...");
                QString qstring = "CREATE DATABASE " + settings.value("Database/Database").toString();
                if (query.exec(qstring)) {
                    statusbar->showMessage("Erfolgreich!");
                    cont = 1;
                } else  {
                    statusbar->showMessage("Datenbank konnte nicht erstellt werden!");
                    return false;
                }
            } else {
                cont = 1;
            }
            if (cont == 1) {
                statusbar->showMessage("Datenbank vorhanden! - In TurnFix-Datenbank wechseln...");
                db.setDatabaseName(settings.value("Database/Database").toString());
                db.close();
                if(db.open()) {
                    statusbar->showMessage("Verbindung erfolgreich hergestellt!");
                    datname = settings.value("Database/Database").toString();
                    return true;
                } else {
                    statusbar->showMessage("Fehler beim wechsel in die TurnFix-Datenbank!");
                    return false;
                }
            }
        } else {
            statusbar->showMessage("Fehler! Der angegeben Benutzer verfügt nicht über die nötigen Rechte!");
            return false;
        }
    } else {
        statusbar->showMessage("Fehler beim Verbinden zum SQL-Server. Überprüfen Sie die angegebenen Daten!");
        return false;
    }
    return false;
}

void DBCheck::updateFunctions() {
    for (int f=0; f<7; f++) {
        if (QVariant(ft_model->data(ft_model->index(f,1))).toString() != "aktuell") {
            QFile file(":/sql/function-" + functions[f] + ".sql");
            QString line;
            QSqlQuery query;

            if ( file.open(QIODevice::ReadOnly) ) {
                QTextStream t( &file );
                while ( !t.atEnd() ) {
                    line = t.readLine();
                    if (query.exec(line)) {
                        statusbar->showMessage("Funktion erfolgreich aktualisiert");
                    } else {
                        statusbar->showMessage("Fehler beim aktualisieren der Funktion!");
                        break;
                    }
                }
                file.close();
            }
        }
    }
    check_versions();
}

void DBCheck::updateTables() {
    for (int t=0; t<tables.size(); t++) {
        if (QVariant(tb_model->data(tb_model->index(t,1))).toString() != "0.9.2") {
            bool wait=true;
            for (int i=0; i<versions.size(); i++) {
                if (QVariant(tb_model->data(tb_model->index(t,1))).toString() == versions[i] || QVariant(tb_model->data(tb_model->index(t,1))).toString() == "existiert nicht") wait = false;
                if (versions[i] == "0.9.2") wait = true;
                if (wait == false) {
                    QFile file(":/sql/update-" + versions[i] + "-" + versions[i+1] + "-" + tables[t] + ".sql");
                    if (file.exists()) {
                        QString line;
                        QSqlQuery query;
                        if ( file.open(QIODevice::ReadOnly) ) {
                            QTextStream t( &file );
                            while ( !t.atEnd() ) {
                                line = t.readLine();
                                if (query.exec(line)) {
                                    statusbar->showMessage("Tabelle erfolgreich aktualisiert");
                                } else {
                                    statusbar->showMessage("Fehler beim aktualisieren der Tabelle!");
                                    break;
                                }
                            }
                            file.close();
                        }
                    }
                }
            }
        }
    }
    check_versions();
}

void DBCheck::backup() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("TurnFix Backup-Datei (*.tba)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("tba");
    if(dialog.exec()) {
        QDomDocument doc("TurnFixBackup");
        QDomElement root = doc.createElement("Tabellen");
        doc.appendChild(root);
        for (int i=0;i<tables.size();i++) {
            QDomElement dotab = doc.createElement("Tabelle");
            root.appendChild(dotab);
            QDomElement dover = doc.createElement("Version");
            dotab.appendChild(dover);
            QDomText dtver = doc.createTextNode(QVariant(tb_model->data(tb_model->index(order[i],1))).toString());
            dover.appendChild(dtver);
            QDomElement donam = doc.createElement("Name");
            dotab.appendChild(donam);
            QDomText dtnam = doc.createTextNode(tables[order[i]]);
            donam.appendChild(dtnam);
            QDomElement dodat = doc.createElement("Data");
            dotab.appendChild(dodat);
            QString sqlquery = "SELECT * FROM " + tables[order[i]];
            QSqlQuery query;
            query.prepare(sqlquery);
            query.exec();
            while (query.next()) {
                QString data;
                QDomElement dtdat = doc.createElement("DataSet");
                dodat.appendChild(dtdat);
                QSqlRecord rec = query.record();
                for (int d=0; d<rec.count(); d++) {
                    data = data + query.value(d).toString();
                    if (d!=(rec.count()-1)) data = data + ";;";
                }
                QDomText dtrow = doc.createTextNode(data);
                dtdat.appendChild(dtrow);
            }

        }
        QFile file( dialog.selectedFiles().at(0) );
        file.open( QIODevice::WriteOnly );
        QTextStream stream( &file );
        stream << doc.toString().toLatin1();
        file.close();
    }
}

void DBCheck::restore() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("TurnFix Backup-Datei (*.tba)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("tba");
    if(dialog.exec()) {
        QMessageBox msg(QMessageBox::Question, "Datenbank wiederherstellen", "Wollen sie die Datenbank wirklich wiederherstellen? Alle momentan vorhandenen Daten werden gelöscht!",QMessageBox::Ok | QMessageBox::Cancel);
        if(msg.exec() == QMessageBox::Ok) {
            for (int i=0; i<tables.size(); i++) {
                QString querystring = "DELETE FROM " + tables[order[14-i]];
                QSqlQuery query;
                query.exec(querystring);
            }
            QFile file( dialog.selectedFiles().at(0) );
            file.open(QIODevice::ReadOnly);
            QTextCodec *uni = QTextCodec::codecForName("ISO 8859-1");
            QTextStream xmlcontent(&file);
            xmlcontent.setAutoDetectUnicode(false);
            xmlcontent.setCodec(uni);
            QDomDocument doc("Tabellen");
            doc.setContent(xmlcontent.readAll());
            file.close();
            QDomElement docElem = doc.documentElement();
            QDomNode n = docElem.firstChild();
            while(!n.isNull()) {
                QDomNode m = n.firstChild();
                QString table;
                int l=1;
                while(!m.isNull()) {
                    if (l<versions.size()) {
                        QDomElement e = m.toElement();
                        if(!e.isNull()) {
                            if (e.tagName() == "Name") {
                                table = e.text();
                            }
                        }
                    } else {
                        QDomNode o = m.firstChild();
                        while(!o.isNull()) {
                            QDomElement e2 = o.toElement();
                            if(!e2.isNull()) {
                                QStringList list = e2.text().split(";;");
                                QString sqlquery = "INSERT INTO " + table + " VALUES (";
                                for (int s=0; s<list.size(); s++) {
                                    sqlquery = sqlquery + "?";
                                    if (s<(list.size()-1)) {
                                        sqlquery = sqlquery + ",";
                                    }
                                }
                                sqlquery = sqlquery + ")";
                                QSqlQuery insertquery;
                                insertquery.prepare(sqlquery);
                                for (int s=0; s<list.size(); s++) {
                                    if (list.at(s).length() == 0) {
                                        insertquery.bindValue(s,QVariant(QVariant::String));
                                    } else {
                                        insertquery.bindValue(s,list.at(s));
                                    }
                                }
                                insertquery.exec();
                            }
                            o = o.nextSibling();
                        }
                    }
                    m = m.nextSibling();
                    l++;
                }
                n = n.nextSibling();
            }
            for (int i=0; i<tables.size(); i++) {
                QString maxstring = "SELECT MAX(" + table_data[i][2].at(0) + ") FROM " + tables[i];
                QSqlQuery max;
                max.exec(maxstring);
                max.next();
                int maxval = max.value(0).toInt();
                QString querystring = "SELECT pg_catalog.setval('" + tables[i] + "_" + table_data[i][2].at(0) + "_seq', " + QString().setNum(maxval+1) + ", true)";
                QSqlQuery query;
                query.exec(querystring);
            }
        }
    }
}

void DBCheck::install() {
    dropdb();
    check_versions();
    QFile file(":/sql/install.sql");
    QString line;
    QSqlQuery query;
    if ( file.open(QIODevice::ReadOnly) ) {
        QTextStream t( &file );
        while ( !t.atEnd() ) {
            line = t.readLine();
            if (query.exec(line)) {
                statusbar->showMessage("Datenbank erfolgreich installiert!");
            } else {
                statusbar->showMessage("Fehler beim erstellen der Datenbank!");
                break;
            }
        }
        file.close();
    }
    check_versions();
}

void DBCheck::dropdb() {
    QFile file(":/sql/drop.sql");
    QString line;
    QSqlQuery query;
    if ( file.open(QIODevice::ReadOnly) ) {
        QTextStream t( &file );
        while ( !t.atEnd() ) {
            line = t.readLine();
            query.exec(line);
        }
        file.close();
    }
}
