#include "header/meldung.h"
#include "../../global/header/_global.h"

void Meldung::printContent() {
    QString pgExtra;
    if (_global::getDBTyp()==0) {
        pgExtra = "::text";
    }
    QString sortstring;
    if (selectClub) {
        sortstring = _global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname, tfx_gruppen.var_name";
    } else {
        sortstring = "tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_mannschaften.int_nummer, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname, tfx_gruppen.var_name";
    }
    QSqlQuery query2;
    query2.prepare("SELECT "
                    "CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN 'M' || tfx_mannschaften.int_startnummer ELSE tfx_wertungen.int_startnummer"+pgExtra+" END, "
                    "CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN tfx_vereine.var_name ELSE CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN tfx_gruppen.var_name ELSE " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END || CASE WHEN bol_startet_nicht='true' THEN ' (fehlt)' ELSE '' END END END, "
                    "CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN tfx_mannschaften.int_nummer || '. Mannschaft' ELSE tfx_vereine.var_name END, "
                    "tfx_wettkaempfe.var_nummer, "
                    "CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN tfx_mannschaften.int_mannschaftenid"+pgExtra+" ELSE CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN tfx_wertungen.int_gruppenid"+pgExtra+" ELSE "+_global::date("dat_geburtstag",2)+" END END, "
                    "tfx_vereine.int_vereineid "
                    "FROM tfx_wertungen "
                    "LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) "
                    "LEFT JOIN tfx_mannschaften ON tfx_wertungen.int_mannschaftenid = tfx_mannschaften.int_mannschaftenid "
                    "LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid "
                    "INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid "
                    "INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid "
                    "WHERE int_veranstaltungenid=? "
                    "AND tfx_vereine.int_vereineid IN ("+_global::intListToString(vereinNumbers)+") "
                    "ORDER BY "+sortstring);
    query2.bindValue(0,_global::checkHWK());
    query2.exec();
    int pre=0;
    QString lastWK="";
    QString lastTN="";
    while (query2.next()) {
        if (lastTN == query2.value(0).toString()) continue;
        currWK = query2.value(3).toString();
        double skip=0.0;
        if (_global::checkTyp(query2.value(3).toString())==0) {
            skip += 4.7;
        } else if (_global::checkTyp(query2.value(3).toString())==2) {
            QSqlQuery groupq;
            groupq.prepare("SELECT int_teilnehmerid FROM tfx_gruppen_x_teilnehmer WHERE int_gruppenid=?");
            groupq.bindValue(0,query2.value(4).toInt());
            groupq.exec();
            skip += ((_global::querySize(groupq)/4)+1)*3.4+4.8;
        } else {
            QSqlQuery teamq;
            teamq.prepare("SELECT " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_mannschaftenid=? AND int_runde=? ORDER BY int_mannschaftenid");
            teamq.bindValue(0,query2.value(4).toInt());
            teamq.bindValue(1,_global::getRunde());
            teamq.exec();
            skip += ((_global::querySize(teamq)/4)+1)*3.4+4.8;
        }
        bool newPageCreated = false;
        if (pre != query2.value(5).toInt() && pre != 0 && selectClub) {
            newPage();
            newPageCreated = true;
        }
        checkWKChange(currWK,lastWK,skip,newPageCreated);
        setPrinterFont(10);
        QString jg;
        if (_global::checkTyp(query2.value(3).toString())==0) jg = query2.value(4).toString();
        QString verein;
        if (_global::checkTyp(query2.value(3).toString())==1) verein = query2.value(1).toString(); else verein = query2.value(2).toString();
        drawStandardRow(query2.value(0).toString() + "  ",query2.value(1).toString(),jg,query2.value(2).toString(),"",readDetailInfo(false,verein));
        if (_global::checkTyp(query2.value(3).toString())==1 || _global::checkTyp(query2.value(3).toString())==2) {
            yco -= mmToPixel(0.8);
            setPrinterFont(8);
            QSqlQuery teamq2;
            switch (_global::checkTyp(query2.value(3).toString())) {
            case 1: {
                    teamq2.prepare("SELECT " + _global::nameFormat() + " || ' (' || "+_global::date("dat_geburtstag",2)+" || ')' || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END || CASE WHEN bol_startet_nicht THEN ' (fehlt)' ELSE '' END FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_mannschaftenid=? AND int_runde=? ORDER BY bol_ak, var_nachname, var_vorname");
                    teamq2.bindValue(1,_global::getRunde());
                } break;
            case 2: teamq2.prepare("SELECT " + _global::nameFormat() + " || ' (' || "+_global::date("dat_geburtstag",2)+" || ')' FROM tfx_gruppen_x_teilnehmer INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_gruppenid=? ORDER BY var_nachname, var_vorname"); break;
            }
            teamq2.bindValue(0,query2.value(4).toString());
            teamq2.exec();
            int column=0;
            while (teamq2.next()) {
                painter.drawText(QRectF(pr.x()+mmToPixel(10.6)+column*((pr.width()-pr.x()-mmToPixel(10.6))/4), yco, ((pr.width()-pr.x()-mmToPixel(10.6))/4), QFontMetricsF(painter.font()).height()),teamq2.value(0).toString(),QTextOption(Qt::AlignVCenter));
                column++;
                if (column == 4) {
                    column = 0;
                    yco += mmToPixel(3.2);
                }
            }
            if (column != 0) yco += mmToPixel(3.2);
            setPrinterFont(10);
            yco += mmToPixel(2.6);
        }
        pre = query2.value(5).toInt();
        lastWK = query2.value(3).toString();
        lastTN = query2.value(0).toString();
    }
    finishPrint();
}
