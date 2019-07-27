#include "individual.h"
#include "model/entity/competition.h"
#include "model/entitymanager.h"
#include "model/repository/competitionrepository.h"
#include "src/global/header/_global.h"
#include "src/global/header/result_calc.h"

void Individual::printContent() {
    for (int i=0;i<wkNumbers.size();i++) {
        currWK = wkNumbers.at(i);
        Competition *competition = m_em->competitionRepository()->fetchByNumber(m_event, currWK);

        if (newPageWK && i > 0) {
            newPage();
        }
        if (checkFitPage(mmToPixel(35.0),currWK)) {
            printDescriptor(currWK);
        }
        QList<QStringList> rlist = Result_Calc::resultArrayNew(competition, vereinNumbers, -1, 0, false, detailQuery);
        for (int i=0;i<rlist.size();i++) {
            checkFitPage(mmToPixel(4.8),currWK,true);
            setPrinterFont(9);
            if (i%2 != 0) drawHighlightRect(yco);
            QString jg;
            QString verein;
            if (competition->type() == 0)
                jg = rlist.at(i).at(3);
            if (competition->type() == 1) {
                verein = rlist.at(i).at(1);
            } else {
                verein = rlist.at(i).at(2);
            }
            QSqlQuery disCountQuery;
            disCountQuery.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_name, int_berechnung, var_einheit, var_kurz1, var_maske, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN generate_series(0,1) ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE var_nummer=? AND int_veranstaltungenid=?");
            disCountQuery.bindValue(0,currWK);
            disCountQuery.bindValue(1, this->m_event->mainEvent()->id());
            disCountQuery.exec();
            QString res;
            if (_global::querySize(disCountQuery) == 1) {
                disCountQuery.next();
                QSqlQuery query4;
                query4.prepare("SELECT rel_leistung FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? ORDER BY rel_leistung DESC LIMIT 1");
                query4.bindValue(0,rlist.at(i).last());
                query4.bindValue(1,disCountQuery.value(0).toInt());
                query4.exec();
                query4.next();
                res = _global::strLeistung(query4.value(0).toDouble(),disCountQuery.value(3).toString(),disCountQuery.value(5).toString(),disCountQuery.value(2).toInt())+disCountQuery.value(3).toString();
            } else {
                res = rlist.at(i).at(rlist.at(i).size()-2);
            }
            drawStandardRow(rlist.at(i).at(0) + "  ",
                                   rlist.at(i).at(1),
                                   jg,
                                   rlist.at(i).at(2),
                                   res,
                                   readDetailInfo(false,verein)
                                   );
        }
        yco += mmToPixel(5.0);
    }
    finishPrint();
}

void Individual::printSubHeader() {
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
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += 1;
}
