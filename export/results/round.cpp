#include "round.h"
#include "model/entity/competition.h"
#include "model/entitymanager.h"
#include "model/repository/competitionrepository.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"

bool Round::useExtraScore = false;

void Round::setUseExtraScore(bool set) {
    useExtraScore = set;
}

void Round::printContent() {
    for (int i=0;i<wkNumbers.size();i++) {
        currWK = wkNumbers.at(i);

        Competition *competition = m_em->competitionRepository()->fetchByNumber(this->m_event,
                                                                                currWK);

        if (newPageWK && i > 0) {
            newPage();
        }
        if (checkFitPage(mmToPixel(35.0),currWK)) {
            printDescriptor(currWK);
        }
        QList<QStringList> rlist = Result_Calc::roundResultArrayNew(competition, useExtraScore, detailQuery);
        for (int i=0;i<rlist.size();i++) {
            checkFitPage(mmToPixel(8.7),currWK,true);

            setPrinterFont(9);
            if (i%2 != 0) drawHighlightRect(yco);
            QString verein;
            QString jg="";
            int add=0;
            if (competition->type() == 1) {
                verein = rlist.at(i).at(1);
            } else {
                verein = rlist.at(i).at(2);
                jg = rlist.at(i).at(3);
                add = 1;
            }
            drawStandardRow(rlist.at(i).at(0) + "  ",rlist.at(i).at(1),jg,rlist.at(i).at(2),"",readDetailInfo(false,verein));
            if (i%2 != 0) drawHighlightRect(yco);
            drawStandardRow("","","","",rlist.at(i).at(rlist.at(i).size()-3),"");
            yco -= fontHeight;
            setPrinterFont(9);
            for (int cc=0;cc<(rlist.at(i).size()-6-add);cc=cc+2) {
                int x = pr.x()+mmToPixel(10.6)+(cc/2)*mmToPixel(16)+(cc/2)*mmToPixel(1.1);
                painter.drawText(QRectF(x, yco, mmToPixel(16), mmToPixel(4.0)),rlist.at(i).at(cc+add+3),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                if (useExtraScore) {
                    yco += mmToPixel(4.0);
                    if (i%2 != 0 && cc==0) drawHighlightRect(yco);
                    painter.drawText(QRectF(x, yco, mmToPixel(16), mmToPixel(4.0)),rlist.at(i).at(cc+add+4),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                    yco -= mmToPixel(4.0);
                }
            }

            if (useExtraScore) {
                yco += mmToPixel(4.0);
                drawStandardRow("","","","",rlist.at(i).at(rlist.at(i).size()-2),"");
            } else {
                yco += mmToPixel(5.0);
            }
        }
        yco += mmToPixel(5.0);
    }
    finishPrint();
}

void Round::printSubHeader() {
    Competition *competition = m_em->competitionRepository()->fetchByNumber(this->m_event, currWK);

    setPrinterFont(10);
    QString jg;

    if (competition->type() == 0)
        jg = "Jg.";
    if (competition->type() == 0 || competition->type() == 2) {
        drawStandardRow("Platz","Name",jg,"Verein","Punkte",readDetailInfo(true));
    } else {
        drawStandardRow("Platz","Verein","","Mannschaft","Punkte",readDetailInfo(true));
    }
    QSqlQuery rnd;
    rnd.prepare("SELECT "+_global::date("dat_von",10)+" FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=? OR int_hauptwettkampf=? AND bol_rundenwettkampf='true' ORDER BY int_runde");
    rnd.bindValue(0, this->m_event->mainEvent()->id());
    rnd.bindValue(1, this->m_event->mainEvent()->id());
    rnd.exec();
    setPrinterFont(7);
    while (rnd.next()) {
        int x = pr.x()+mmToPixel(10.6)+(rnd.at())*mmToPixel(16)+(rnd.at())*mmToPixel(1.1);
        painter.drawText(QRectF(x, yco, mmToPixel(16), mmToPixel(4.0)),rnd.value(0).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
    }
    yco += mmToPixel(4.0);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += 1;
}
