#include "summary.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"

void Summary::print(QPrinter *printer) {
    setTypeString("Bericht");
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void Summary::printContent() {
    setPrinterFont(11,true);
    QSqlQuery query;
    query.prepare("SELECT int_veranstaltungenid, var_veranstalter, var_vorname || ' ' || var_nachname, tfx_personen.var_adresse, tfx_personen.var_plz, tfx_personen.var_ort, tfx_wettkampforte.var_name, tfx_wettkampforte.var_adresse, tfx_wettkampforte.var_plz, tfx_wettkampforte.var_ort, int_edv, int_kampfrichter, int_helfer FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) LEFT JOIN tfx_personen ON int_ansprechpartner = int_personenid WHERE int_veranstaltungenid=?");
    query.bindValue(0, this->m_event->id());
    query.exec();
    query.next();
    drawTextLine("Veranstalter",0,false);
    drawTextLine("Veranstaltungsort", pr.width()/2);
    setPrinterFont(11);
    drawTextLine(query.value(1).toString(),0,false);
    drawTextLine(query.value(6).toString(), pr.width()/2);
    drawTextLine(query.value(2).toString(),0,false);
    drawTextLine(query.value(7).toString(), pr.width()/2);
    drawTextLine(query.value(3).toString(),0,false);
    drawTextLine(query.value(8).toString() + " " + query.value(9).toString(), pr.width()/2);
    drawTextLine(query.value(4).toString() + " " + query.value(5).toString());
    yco += mmToPixel(2.1);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(2.1);
    setPrinterFont(11,true);
    drawTextLine("Zahl der angetretenen Teilnehmer");
    setPrinterFont(11);
    QSqlQuery query3;
    query3.prepare("SELECT var_nummer, var_name, int_wettkaempfeid FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query3.bindValue(0, this->m_event->mainEvent()->id());
    query3.exec();
    while (query3.next()) {
        painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"WK Nr. " + query3.value(0).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(26.5), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),query3.value(1).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.width()/2, yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),_global::wkBez(this->m_event, query3.value(0).toString()),QTextOption(Qt::AlignVCenter));
        QSqlQuery query4;
        query4.prepare("SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=? AND int_runde=?");
        query4.bindValue(0, query3.value(2).toInt());
        query4.bindValue(1, this->m_event->round());
        query4.exec();
        query4.next();
        painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(),QFontMetricsF(painter.font()).height()),query4.value(0).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignRight));
        yco += mmToPixel(5.3);
    }
    drawTextLine("Anzahl Vereine",0,false);
    QSqlQuery query2;
    query2.prepare("SELECT COUNT(*), CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN v1.var_name ELSE v2.var_name END AS cname FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_vereine AS v1 ON v1.int_vereineid = tfx_teilnehmer.int_vereineid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_vereine AS v2 ON v2.int_vereineid = tfx_gruppen.int_vereineid WHERE int_wettkaempfeid IN (SELECT int_wettkaempfeid FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?) AND int_runde=? GROUP BY cname");
    query2.bindValue(0, this->m_event->mainEvent()->id());
    query2.bindValue(1, this->m_event->round());
    query2.exec();
    drawTextLine(QString().setNum(_global::querySize(query2)),pr.x()+mmToPixel(39.7));
    yco += mmToPixel(2.1);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(2.1);
    setPrinterFont(11,true);
    drawTextLine("Zahl der eingesetzen Helfer");
    setPrinterFont(11);
    drawTextLine("Wettkampfbüro/EDV",0,false);
    drawTextLine(query.value(10).toString(),pr.x()+mmToPixel(39.7));
    drawTextLine("Kampfrichter",0,false);
    drawTextLine(query.value(11).toString(),pr.x()+mmToPixel(39.7));
    drawTextLine("Helfer",0,false);
    drawTextLine(query.value(12).toString(),pr.x()+mmToPixel(39.7));
    setPrinterFont(11,true);
    drawTextLine("Summe",0,false);
    drawTextLine(QString().setNum(query.value(10).toInt()+query.value(11).toInt()+query.value(12).toInt()),pr.x()+mmToPixel(39.7));
    setPrinterFont(11);
    finishPrint();
}
