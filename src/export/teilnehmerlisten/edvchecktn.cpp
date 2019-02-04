#include "header/edvchecktn.h"
#include "../../global/header/_global.h"

void EDVCheckTN::printContent() {
    QSqlQuery query2;
    query2.prepare("SELECT int_startnummer, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END ELSE tfx_gruppen.var_name END, tfx_vereine.var_name, var_nummer, "+_global::date("dat_geburtstag",2)+" FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid WHERE int_veranstaltungenid=? AND int_runde=? AND bol_startet_nicht='false' ORDER BY var_nummer, "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, var_nachname, var_vorname");
    query2.bindValue(0,_global::checkHWK());
    query2.bindValue(1,_global::getRunde());
    query2.exec();
    QString lastWK="";
    while (query2.next()) {
        currWK = query2.value(3).toString();
        checkWKChange(currWK,lastWK,6.6);

        setPrinterFont(10);
        painter.drawLine(QPointF(pr.width()-pr.x(),yco-mmToPixel(1.3)),QPointF(pr.width()-pr.x(),yco-mmToPixel(1.3)+mmToPixel(6.6)));
        painter.drawLine(QPointF(pr.width()-pr.x()-mmToPixel(5.8),yco-mmToPixel(1.3)),QPointF(pr.width()-pr.x()-mmToPixel(5.8),yco-mmToPixel(1.3)+mmToPixel(6.6)));
        drawStandardRow(query2.value(0).toString() + "  ",query2.value(1).toString(),query2.value(4).toString(),query2.value(2).toString(),"",readDetailInfo(false,query2.value(2).toString()));
        painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
        yco += mmToPixel(1.3);
        lastWK = query2.value(3).toString();
    }
    finishPrint();
}

void EDVCheckTN::printSubHeader() {
    Liste::printSubHeader();
    yco -= fontHeight+mmToPixel(1.0)+1;
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(5.3)), yco, mmToPixel(5.3), QFontMetricsF(painter.font()).height()),"Ok",QTextOption(Qt::AlignVCenter));
    yco += fontHeight+mmToPixel(1.0)+1;
}
