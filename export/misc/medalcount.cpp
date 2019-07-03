#include "medalcount.h"
#include "model/objects/competition.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"

void MedalCount::print(QPrinter *printer) {
    ids.clear();
    gesamt.clear();
    gold.clear();
    silber.clear();
    bronze.clear();
    namen.clear();
    QSqlQuery query2;
    query2.prepare("SELECT tfx_vereine.int_vereineid, tfx_vereine.var_name, (SELECT COUNT(*) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid WHERE (tfx_teilnehmer.int_vereineid=tfx_vereine.int_vereineid OR tfx_gruppen.int_vereineid=tfx_vereine.int_vereineid OR tfx_mannschaften.int_vereineid=tfx_vereine.int_vereineid) AND int_veranstaltungenid=? AND bol_startet_nicht='false') FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid LEFT JOIN tfx_mannschaften ON tfx_wertungen.int_mannschaftenid = tfx_mannschaften.int_mannschaftenid INNER JOIN tfx_vereine ON tfx_teilnehmer.int_vereineid = tfx_vereine.int_vereineid OR tfx_gruppen.int_vereineid = tfx_vereine.int_vereineid OR tfx_mannschaften.int_vereineid = tfx_vereine.int_vereineid WHERE int_veranstaltungenid=? GROUP BY tfx_vereine.int_vereineid, tfx_vereine.var_name, tfx_vereine.int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1")+", tfx_vereine.var_name");
    query2.bindValue(0, this->event->getMainEventId());
    query2.bindValue(1, this->event->getMainEventId());
    query2.exec();
    while (query2.next()) {
        ids.append(query2.value(0).toInt());
        namen[query2.value(0).toInt()] = query2.value(1).toString();
        gesamt[query2.value(0).toInt()] = query2.value(2).toInt();
    }
    for (int i=0;i<wkNumbers.size();i++) {
        QString currWK = wkNumbers.at(i);
        Competition *competition = Competition::getByNumber(this->event, currWK);

        QList<QStringList> rlist = Result_Calc::resultArrayNew(competition);
        for(int j=0;j<rlist.size();j++) {
            QStringList data = rlist.at(j);
            QSqlQuery query;
            if (competition->getType() == 1) {
                query.prepare("SELECT int_vereineid FROM tfx_mannschaften WHERE int_mannschaftenid=?");
            } else if (competition->getType() == 1) {
                query.prepare("SELECT int_vereineid FROM tfx_wertungen INNER JOIN tfx_gruppen USING (int_gruppenid) WHERE int_wertungenid=?");
            } else {
                query.prepare("SELECT int_vereineid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_wertungenid=?");
            }
            query.bindValue(0,data.last());
            query.exec();
            query.next();
            int platz;
            if (data.at(0).right(2) == " Q") {
                platz = data.at(0).left(data.at(0).length()-2).toInt();
            } else {
                platz = data.at(0).toInt();
            }
            if (platz > 3) break;
            switch (platz) {
            case 1: gold[query.value(0).toInt()]++; break;
            case 2: silber[query.value(0).toInt()]++; break;
            case 3: bronze[query.value(0).toInt()]++; break;
            }
        }
    }
    for (int i=0;i<ids.size();i++) {
        for (int j=0;j<(i+1);j++) {
            if (gold[ids.at(i)]>gold[ids.at(j)] || (gold[ids.at(i)]==gold[ids.at(j)] && silber[ids.at(i)]>silber[ids.at(j)]) || (gold[ids.at(i)]==gold[ids.at(j)] && silber[ids.at(i)]==silber[ids.at(j)] && bronze[ids.at(i)]>bronze[ids.at(j)])) {
                ids.swapItemsAt(i,j);
            }
        }
    }
    setTypeString("Medallien");
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void MedalCount::printContent() {
    printSubHeader();
    for (int i=0;i<ids.size();i++) {
        if (yco+mmToPixel(7.9) > max_yco) {
            newPage();
            printSubHeader();
        }
        setPrinterFont(10);
        if (i%2 != 0) drawHighlightRect(yco,mmToPixel(5.0));
        painter.drawText(QRectF(pr.x(), yco, mmToPixel(79.4), QFontMetricsF(painter.font()).height()),QString(namen.value(ids.at(i))),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(80.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),QString::number(gold.value(ids.at(i))),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(100.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),QString::number(silber.value(ids.at(i))),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(120.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),QString::number(bronze.value(ids.at(i))),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(140.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),QString::number(gold.value(ids.at(i))+silber.value(ids.at(i))+bronze.value(ids.at(i))),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(160.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),QString::number(gesamt.value(ids.at(i))),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        yco += mmToPixel(5.3);
    }
    finishPrint();
}

void MedalCount::printSubHeader() {
    setPrinterFont(10,true);
    painter.drawText(QRectF(pr.x(), yco, mmToPixel(79.4), QFontMetricsF(painter.font()).height()),"Verein",QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(80.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),"Gold",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(100.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),"Silber",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(120.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),"Bronze",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(140.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),"Summe",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(160.0), yco,mmToPixel(20.0), QFontMetricsF(painter.font()).height()),"Starter",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    yco += mmToPixel(5.0);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(0.3);
}
