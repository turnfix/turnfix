#include "header/vereinsliste.h"
#include "../../global/header/_global.h"

void Vereinsliste::print(QPrinter *printer) {
    setTypeString("Vereine");
    Drucken::print(printer);
    printHeadFoot();
    printContent();
}

void Vereinsliste::printContent() {
    printSubHeader();
    QSqlQuery query2;
    query2.prepare("SELECT tfx_vereine.var_name, (SELECT COUNT(*) FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_wettkaempfe ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_vereineid=tfx_vereine.int_vereineid AND int_veranstaltungenid=?), (SELECT COUNT(*) FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_vereineid=tfx_vereine.int_vereineid AND int_veranstaltungenid=?), (SELECT COUNT(*) FROM tfx_gruppen INNER JOIN tfx_wertungen USING (int_gruppenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_vereineid=tfx_vereine.int_vereineid AND int_veranstaltungenid=?) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid LEFT JOIN tfx_mannschaften ON tfx_wertungen.int_mannschaftenid = tfx_mannschaften.int_mannschaftenid INNER JOIN tfx_vereine ON tfx_teilnehmer.int_vereineid = tfx_vereine.int_vereineid OR tfx_gruppen.int_vereineid = tfx_vereine.int_vereineid OR tfx_mannschaften.int_vereineid = tfx_vereine.int_vereineid WHERE int_veranstaltungenid=? GROUP BY tfx_vereine.int_vereineid, tfx_vereine.var_name, tfx_vereine.int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name");
    query2.bindValue(0,_global::checkHWK());
    query2.bindValue(1,_global::checkHWK());
    query2.bindValue(2,_global::checkHWK());
    query2.bindValue(3,_global::checkHWK());
    query2.exec();
    while (query2.next()) {
        if (yco+mmToPixel(7.9) > max_yco) {
            newPage();
            printSubHeader();
        }
        setPrinterFont(10);
        if (query2.at()%2 != 0) drawHighlightRect(yco,mmToPixel(5.0));
        painter.drawText(QRectF(pr.x(), yco, mmToPixel(79.4), QFontMetricsF(painter.font()).height()),QString(query2.value(0).toString() + "  "),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(79.4), yco,mmToPixel(26.5), QFontMetricsF(painter.font()).height()),QString(query2.value(1).toString()),QTextOption(Qt::AlignVCenter));
        if (query2.value(2).toInt() > 0) painter.drawText(QRectF(pr.x()+mmToPixel(105.9), yco, mmToPixel(26.5), QFontMetricsF(painter.font()).height()),QString(query2.value(2).toString()),QTextOption(Qt::AlignVCenter));
        if (query2.value(3).toInt() > 0) painter.drawText(QRectF(pr.x()+mmToPixel(132.4), yco, mmToPixel(26.5), QFontMetricsF(painter.font()).height()),QString(query2.value(3).toString()),QTextOption(Qt::AlignVCenter));
        yco += mmToPixel(5.3);
    }
    finishPrint();
}

void Vereinsliste::printSubHeader() {
    setPrinterFont(10,true);
    painter.drawText(QRectF(pr.x(), yco, mmToPixel(79.4), QFontMetricsF(painter.font()).height()),"Verein",QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(79.4), yco, mmToPixel(26.5), QFontMetricsF(painter.font()).height()),"Teilnehmer",QTextOption(Qt::AlignVCenter));
    QSqlQuery query;
    query.prepare("SELECT COUNT(CASE WHEN int_mannschaftenid IS NOT NULL THEN 1 END), COUNT(CASE WHEN int_gruppenid IS NOT NULL THEN 1 END) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=?");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();
    if (query.value(0).toInt() > 0) painter.drawText(QRectF(pr.x()+mmToPixel(105.9), yco, mmToPixel(26.5), QFontMetricsF(painter.font()).height()),"Mannschaften",QTextOption(Qt::AlignVCenter));
    if (query.value(1).toInt() > 0) painter.drawText(QRectF(pr.x()+mmToPixel(132.4), yco, mmToPixel(26.5), QFontMetricsF(painter.font()).height()),"Gruppen",QTextOption(Qt::AlignVCenter));
    yco += mmToPixel(5.0);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(0.3);
}
