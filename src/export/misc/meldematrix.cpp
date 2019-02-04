#include <math.h>
#include "header/meldematrix.h"
#include "../../global/header/_global.h"
#include <QDebug>

bool MeldeMatrix::teamMode = false;

void MeldeMatrix::print(QPrinter *printer) {
    setTypeString("Meldematrix");
    Drucken::print(printer);
    printHeadFoot();
    printContent();
}

void MeldeMatrix::printContent() {
    QSqlQuery query;
    query.prepare("SELECT int_wettkaempfeid, var_nummer FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query.bindValue(0,_global::checkHWK());
    query.exec();
    int count = _global::querySize(query);
    QList<int> ges;
    int perpage = (int)floor((pr.width()-pr.x()-pr.x()-mmToPixel(46.3))/mmToPixel(10.6));
    int pages = (int)ceil(count/floor((pr.width()-pr.x()-pr.x()-mmToPixel(46.3))/mmToPixel(10.6)));
    for (int p=0;p<pages;p++) {
        ges.clear();
        if (p > 0) {
            newPage();
        }
        setPrinterFont(9,true);
        int firstwk = p*perpage;
        int max=0;
        if ((count-firstwk)>perpage) {
            max = perpage;
        } else {
            max = count-firstwk;
        }
        QString querystring = "SELECT tfx_vereine.var_name,";
        int t=0;
        for (int q=firstwk;q<(firstwk+max);q++) {
            query.seek(q);
            if (!teamMode) {
                querystring = querystring + "count(case when var_nummer='" + query.value(1).toString() + "' then tfx_wertungen.int_teilnehmerid end)+count(case when var_nummer='" + query.value(1).toString() + "' then tfx_wertungen.int_gruppenid end),";
            } else {
                querystring = querystring + "count(case when var_nummer='" + query.value(1).toString() + "' then tfx_mannschaften.int_mannschaftenid end),";
            }
            painter.drawText(QRectF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*t, yco, mmToPixel(10.6), QFontMetricsF(painter.font()).height()),"Wk.",QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
            painter.drawText(QRectF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*t, yco+mmToPixel(3.2), mmToPixel(10.6), QFontMetricsF(painter.font()).height()),query.value(1).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
            ges.append(0);
            t++;
        }
        painter.drawText(QRectF(pr.x(), yco, mmToPixel(46.3), QFontMetricsF(painter.font()).height()),"Verein",QTextOption(Qt::AlignVCenter));
        if ((firstwk+max) == count) painter.drawText(QRectF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*max, yco, mmToPixel(10.6), QFontMetricsF(painter.font()).height()),"Ges.",QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
        ges.append(0);
        int start = yco;
        yco += mmToPixel(8.7);
        setPrinterFont(9);
        if (!teamMode) {
            querystring += "count(*) as gesamt FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_wertungen.int_teilnehmerid = tfx_teilnehmer.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid WHERE int_veranstaltungenid=? AND int_runde=? GROUP BY tfx_vereine.var_name, int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1");
        } else {
            querystring += "count(*) as gesamt FROM tfx_mannschaften INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid WHERE int_veranstaltungenid=? GROUP BY tfx_vereine.var_name, int_start_ort ORDER BY "+_global::substring("tfx_vereine.var_name","int_start_ort+1");
        }
        QSqlQuery query2;
        query2.prepare(querystring);
        query2.bindValue(0,_global::checkHWK());
        if (!teamMode) query2.bindValue(1,_global::getRunde());
        query2.exec();
        while(query2.next()) {
            if (yco+mmToPixel(5.3) > max_yco) {
                painter.drawLine(QPointF(pr.x()+mmToPixel(46.3),start),QPointF(pr.x()+mmToPixel(46.3),start+(mmToPixel(5.3)*_global::querySize(query2))+mmToPixel(7.9)));
                setPrinterFont(9,true);
                for (int i=0;i<=max;i++) {
                    if (i<max || (firstwk+max) == count) {
                        painter.drawLine(QPointF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i,start),QPointF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i,start+(mmToPixel(5.3)*_global::querySize(query2))+mmToPixel(7.9)));
                    }
                }
                newPage();
                start = yco;
            }
            setPrinterFont(9,true);
            if (query2.at()%2 != 0) drawHighlightRect(yco-mmToPixel(0.5),mmToPixel(5.0));
            painter.drawText(QRectF(pr.x(), yco, mmToPixel(46.3), QFontMetricsF(painter.font()).height()),query2.value(0).toString(),QTextOption(Qt::AlignVCenter));
            for (int i=0;i<=max;i++) {
                if (i == max) setPrinterFont(9,true);
                if (i < max || (firstwk+max) == count) painter.drawText(QRectF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i, yco, mmToPixel(10.6), QFontMetricsF(painter.font()).height()),query2.value(i+1).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
                ges.replace(i,ges.at(i)+query2.value(i+1).toInt());
                if (i == max) setPrinterFont(9);
            }
            yco += mmToPixel(4.5);
            painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
            yco += mmToPixel(0.8);
        }
        painter.drawLine(QPointF(pr.x()+mmToPixel(46.3),start),QPointF(pr.x()+mmToPixel(46.3),start+(mmToPixel(5.3)*_global::querySize(query2))+mmToPixel(7.9)));
        setPrinterFont(9,true);
        for (int i=0;i<=max;i++) {
            if (i<max || (firstwk+max) == count) {
                painter.drawLine(QPointF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i,start),QPointF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i,start+(mmToPixel(5.3)*_global::querySize(query2))+mmToPixel(7.9)));
                painter.drawText(QRectF(pr.x()+mmToPixel(46.3)+mmToPixel(10.6)*i, yco, mmToPixel(10.6), QFontMetricsF(painter.font()).height()),QString().setNum(ges.at(i)),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
            }
        }
    }
    finishPrint();
}

void MeldeMatrix::setTeamMode(bool tm) {
    teamMode = tm;
}
