#include "header/tabelle.h"
#include "../../global/header/_global.h"
#include "../../global/header/result_calc.h"

void Tabelle::printContent() {
    for (int i=0;i<wkNumbers.size();i++) {
        currWK = wkNumbers.at(i);
        if (_global::checkTyp(currWK) != 1) {
            continue;
        }
        if (newPageWK && i > 0) {
            newPage();
        }
        if (checkFitPage(mmToPixel(35.0),currWK)) {
            printDescriptor(currWK);
        }
        QList<QStringList> rlist = Result_Calc::tabllenArray(currWK);
        for (int i=0;i<rlist.size();i++) {
            int size = rlist.at(i).size();
            checkFitPage(mmToPixel(8.7),currWK,true);

            setPrinterFont(9);
            if (i%2 != 0) drawHighlightRect(yco);

            painter.drawText(QRectF(pr.x(),yco,mmToPixel(20.0),fontHeight),rlist.at(i).at(0),QTextOption(Qt::AlignVCenter));
            painter.drawText(QRectF(pr.x()+mmToPixel(10.6), yco, mmToPixel(79.4), fontHeight),rlist.at(i).at(1)+" "+rlist.at(i).at(2).split(".").at(0),QTextOption(Qt::AlignVCenter));

            painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(35.0),yco,mmToPixel(15.0),fontHeight),rlist.at(i).at(size-4)+" : "+rlist.at(i).at(size-3),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
            painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(20.0),yco,mmToPixel(20.0),fontHeight),rlist.at(i).at(size-2),QTextOption(Qt::AlignVCenter | Qt::AlignRight));

            for (int cc=0;cc<(rlist.at(i).size()-7);cc=cc+2) {
                painter.drawText(QRectF(pr.x()+mmToPixel(55.0)+(cc/2)*mmToPixel(34.0), yco, mmToPixel(17.0), fontHeight),rlist.at(i).at(cc+3),QTextOption(Qt::AlignVCenter | Qt::AlignRight));
                painter.drawText(QRectF(pr.x()+mmToPixel(55.0)+(cc/2)*mmToPixel(34.0)+mmToPixel(17.0), yco, mmToPixel(17.0), fontHeight),rlist.at(i).at(cc+4),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
            }

            yco += mmToPixel(5.0);
        }
        yco += mmToPixel(5.0);
    }
    finishPrint();
}

void Tabelle::printSubHeader() {
    setPrinterFont(10);

    painter.drawText(QRectF(pr.x(),yco,mmToPixel(20.0),fontHeight),"Platz",QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(10.6), yco, mmToPixel(79.4), fontHeight),"Verein",QTextOption(Qt::AlignVCenter));

    painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(35.0),yco,mmToPixel(35.0),fontHeight),"Gesamtpunkte",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));

    QSqlQuery rnd;
    rnd.prepare("SELECT "+_global::date("dat_von",10)+" FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=? OR int_hauptwettkampf=? ORDER BY int_runde");
    rnd.bindValue(0,_global::checkHWK());
    rnd.bindValue(1,_global::checkHWK());
    rnd.exec();
    while (rnd.next()) {
        painter.drawText(QRectF(pr.x()+mmToPixel(55.0)+(rnd.at())*mmToPixel(34.0), yco, mmToPixel(34.0), fontHeight),rnd.value(0).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    }
    yco += mmToPixel(5.0);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(1.0);
}
