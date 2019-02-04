#include "header/urkunde.h"
#include "../../global/header/_global.h"
#include "../../global/header/result_calc.h"

bool Urkunde::eineUrkunde = false;
bool Urkunde::rundenErgebnisse = false;
bool Urkunde::platzWertung = false;
bool Urkunde::einzelErgebnis = false;
int Urkunde::urkundenID = 0;

void Urkunde::print(QPrinter *printer) {
    Drucken::print(printer);
    printContent();
}

void Urkunde::printContent() {
    QList<QStringList> rlist;
    QSqlQuery wkdata;
    wkdata.prepare("SELECT tfx_veranstaltungen.var_name, to_char(dat_von, 'dd.mm.yyyy'), to_char(dat_bis, 'dd.mm.yyyy'), tfx_wettkampforte.var_name, var_ort, tfx_wettkaempfe.var_name, tfx_wettkaempfe.var_nummer, tfx_gaue.var_name, tfx_verbaende.var_name, tfx_laender.var_name FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) INNER JOIN tfx_wettkaempfe USING (int_veranstaltungenid)WHERE tfx_veranstaltungen.int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=?");
    wkdata.bindValue(0,_global::checkHWK());
    wkdata.bindValue(1,selectedTNWK);
    wkdata.exec();
    wkdata.next();
    if (_global::checkTyp(selectedTNWK) == 1 && rundenErgebnisse) {
        rlist = Result_Calc::roundResultArrayNew(selectedTNWK,platzWertung);
    } else {
        int typ = _global::checkTyp(selectedTNWK);
        if (einzelErgebnis) typ = 0;
        rlist = Result_Calc::resultArrayNew(selectedTNWK,typ);
    }
    rlist = Result_Calc::sortRes(rlist);
    for (int i=(rlist.size()-1);i>=0;i--) {
        if (!teilnehmerNumbers.contains(rlist.at(i).last().toInt()) && !einzelErgebnis) {
            rlist.removeAt(i);
        }
    }

    for (int r=0;r<rlist.size();r++) {
        int numOfUrkunden = 1;
        if (_global::checkTyp(selectedTNWK) == 1 && !eineUrkunde && !einzelErgebnis) {
            QSqlQuery teamCount;
            teamCount.prepare("SELECT COUNT(*) FROM tfx_man_x_teilnehmer WHERE int_mannschaftenid=? AND int_runde=?");
            teamCount.bindValue(0,rlist.at(r).last().toInt());
            teamCount.bindValue(1,_global::getRunde());
            teamCount.exec();
            teamCount.next();
            numOfUrkunden = teamCount.value(0).toInt();
        }
        for (int i=0;i<numOfUrkunden;i++) {
            printCustomPage(1,urkundenID,rlist.at(r),selectedTNWK);
            if (!(r==rlist.size()-1 && i==numOfUrkunden-1)) newPage(false);
        }
    }
    finishPrint();
}

void Urkunde::setEineUrkunde(bool set) {
    eineUrkunde = set;
}

void Urkunde::setRundenErgebnisse(bool set) {
    rundenErgebnisse = set;
}

void Urkunde::setPlatzWertung(bool set) {
    platzWertung = set;
}

void Urkunde::setEinzelErgebnis(bool set) {
    einzelErgebnis = set;
}

void Urkunde::setUrkundenID(int set) {
    urkundenID = set;
}

bool Urkunde::getRundenErgebnisse() {
    return rundenErgebnisse;
}
