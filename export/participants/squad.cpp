#include "squad.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"

int Squad::order = 0;
bool Squad::newPageEach = false;

void Squad::setOrder(int set) {
    order = set;
}

void Squad::setNewPageEach(bool set) {
    newPageEach = set;
}

void Squad::printContent() {
    for (int i=0;i<riegenNumbers.size();i++) {
        if (i>0 && newPageEach) newPage();

        currRiege = riegenNumbers.at(i);

        QSqlQuery query2;
        QString sortstring;
        if (order == 0) {
            sortstring = "tfx_wettkaempfe.var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, tfx_mannschaften.int_nummer, bol_ak, tfx_teilnehmer.var_nachname, tfx_teilnehmer.var_vorname, tfx_gruppen.var_name";
        } else {
            sortstring = "tfx_wettkaempfe.var_nummer, tfx_wertungen.int_startnummer";
        }
        query2.prepare("SELECT tfx_wertungen.int_startnummer, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END ELSE tfx_gruppen.var_name END, CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN tfx_vereine.var_name || ' - ' || tfx_mannschaften.int_nummer || '. M.' ELSE tfx_vereine.var_name END, tfx_wettkaempfe.var_nummer, "+_global::date("dat_geburtstag",2)+" FROM tfx_wertungen LEFT JOIN tfx_mannschaften USING (int_mannschaftenid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid=tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid OR tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wertungen.var_riege=? AND int_runde=? AND bol_startet_nicht='false' ORDER BY " + sortstring);
        query2.bindValue(0, this->m_event->mainEvent()->id());
        query2.bindValue(1, currRiege);
        query2.bindValue(2, this->m_event->round());
        query2.exec();
        if (max_yco-yco-mmToPixel(55.0)<0) newPage();

        setPrinterFont(18,true);
        painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), (QFontMetricsF(painter.font()).height())),"Riege "+currRiege,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        yco += QFontMetricsF(painter.font()).height();

        QSqlQuery query3;
        query3.prepare("SELECT var_kurz2 FROM tfx_riegen_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? AND bol_erstes_geraet='true' LIMIT 1");
        query3.bindValue(0, this->m_event->mainEvent()->id());
        query3.bindValue(1, this->m_event->round());
        query3.bindValue(2, currRiege);
        query3.exec();
        if (_global::querySize(query3) > 0) {
            query3.next();
            setPrinterFont(11,true);
            painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), (QFontMetricsF(painter.font()).height())),"1. Gerät: " + query3.value(0).toString(),QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            yco += QFontMetricsF(painter.font()).height();
        }
        yco += mmToPixel(5.0);

        QString lastWK="";
        while (query2.next()) {
            currWK = query2.value(3).toString();

            QSqlQuery query;
            query.prepare("SELECT int_durchgang, int_bahn, bol_info_anzeigen FROM tfx_wettkaempfe WHERE var_nummer=? AND int_veranstaltungenid=? LIMIT 1");
            query.bindValue(0, currWK);
            query.bindValue(1, this->m_event->mainEvent()->id());
            query.exec();
            query.next();
            if (query.value(2).toBool()) {
                setPrinterFont(11,true);
                painter.drawText(QRectF(pr.x(), top_yco-mmToPixel(3.5), mmToPixel(158.9), QFontMetricsF(painter.font()).height()),"Durchgang: " + query.value(0).toString() + " - Bahn: " + query.value(1).toString(),QTextOption(Qt::AlignVCenter));
            }

            checkWKChange(currWK,lastWK,4.8);
            setPrinterFont(10);
            drawStandardRow(query2.value(0).toString() + "  ",query2.value(1).toString(),query2.value(4).toString(),query2.value(2).toString(),"",readDetailInfo(false,query2.value(2).toString()));
            lastWK = query2.value(3).toString();
        }
        yco += mmToPixel(8.0);
    }
    finishPrint();
}
