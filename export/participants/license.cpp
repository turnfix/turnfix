#include "license.h"
#include "src/global/header/_global.h"

void License::printContent() {
    QSqlQuery query2;
    query2.prepare("SELECT int_startnummer, " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END, tfx_vereine.var_name, var_nummer, "+_global::date("dat_geburtstag",2)+", int_startpassnummer FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_veranstaltungenid=? AND int_runde=? AND int_vereineid IN ("+_global::intListToString(vereinNumbers)+") ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name, var_nummer, var_nachname, var_vorname");
    query2.bindValue(0, this->event->mainEventId());
    query2.bindValue(1, this->event->round());
    query2.exec();

    QString lastWK="";
    QString lastClub="";
    while (query2.next()) {
        currWK = query2.value(3).toString();
        if (query2.value(2).toString() != lastClub && query2.at()>0) {
            newPage();
            lastWK = "";
        }
        checkWKChange(currWK,lastWK,6.6);
        setPrinterFont(10);
        painter.drawLine(QPointF(pr.width()-pr.x(),yco-mmToPixel(1.3)),QPointF(pr.width()-pr.x(),yco-mmToPixel(1.3)+mmToPixel(6.6)));
        painter.drawLine(QPointF(pr.width()-pr.x()-mmToPixel(5.8),yco-mmToPixel(1.3)),QPointF(pr.width()-pr.x()-mmToPixel(5.8),yco-mmToPixel(1.3)+mmToPixel(6.6)));
        painter.drawLine(QPointF(pr.width()-pr.x()-mmToPixel(45.5),yco-mmToPixel(1.3)),QPointF(pr.width()-pr.x()-mmToPixel(45.5),yco-mmToPixel(1.3)+mmToPixel(6.6)));
        if (query2.value(5).toString() != "" && query2.value(5).toInt() > 0) painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(45.0)), yco, mmToPixel(39.7), QFontMetricsF(painter.font()).height()),QString(query2.value(5).toString()),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
        drawStandardRow(query2.value(0).toString() + "  ",query2.value(1).toString(),query2.value(4).toString(),query2.value(2).toString());
        painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
        yco += mmToPixel(1.3);
        lastWK = query2.value(3).toString();
        lastClub = query2.value(2).toString();
    }
    finishPrint();
}

void License::printSubHeader() {
    List::printSubHeader();
    yco -= fontHeight+mmToPixel(1.0)+1;
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(45.0)), yco, mmToPixel(39.7), QFontMetricsF(painter.font()).height()),"Startpass-Nr.",QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(5.3)), yco, mmToPixel(5.3), QFontMetricsF(painter.font()).height()),"Ok",QTextOption(Qt::AlignVCenter));
    yco += fontHeight+mmToPixel(1.0)+1;
}
