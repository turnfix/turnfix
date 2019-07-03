#include <QSqlQuery>
#include "model/objects/competition.h"
#include "header/result_calc.h"
#include "header/_global.h"
#include "libs/fparser/fparser.hh"

QList<QStringList> Result_Calc::resultArrayNew(Competition *competition, QList<int> cres, int rnd, int print, bool printAW, QString detailQuery) {
    if (rnd == -1)
        rnd = competition->getEvent()->getRound();
    QSqlQuery wk;
    wk.prepare("SELECT bol_streichwertung, int_qualifikation, int_wertungen, int_wettkaempfeid, bol_sortasc, bol_gerpkt, int_anz_streich, int_typ FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND var_nummer=? LIMIT 1");
    wk.bindValue(0, competition->getEvent()->getMainEventId());
    wk.bindValue(1, competition->getNumber());
    wk.exec();
    wk.next();
    QSqlQuery dis;
    if (competition->getType() == 1 && print == 0) {
        dis.prepare("SELECT int_disziplinenid, int_berechnung, var_formel, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp, rel_max FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE tfx_wettkaempfe.int_wettkaempfeid=? ORDER BY int_sortierung DESC, kp");
    } else {
        dis.prepare("SELECT int_disziplinenid, int_berechnung, var_formel, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp, rel_max FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE tfx_wettkaempfe.int_wettkaempfeid=? ORDER BY int_sortierung, kp");
    }
    dis.bindValue(0,wk.value(3).toInt());
    dis.exec();
    QList< QList<int> > disrows;
    while (dis.next()) {
        QList<int> lst;
        lst.append(dis.at());
        lst.append(0);
        disrows.append(lst);
        if (dis.value(3).toInt()==1) {
            lst.replace(1,1);
            disrows.append(lst);
        }
    }
    QSqlQuery res;
    res.prepare("SELECT tfx_wertungen.int_wertungenid, tfx_wettkaempfe_x_disziplinen.int_disziplinenid, max(tfx_wertungen_details.rel_leistung), max(jr.rel_leistung), tfx_wertungen_details.int_kp FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) LEFT JOIN tfx_wertungen_details ON tfx_wertungen_details.int_disziplinenid = tfx_wettkaempfe_x_disziplinen.int_disziplinenid AND tfx_wertungen_details.int_wertungenid = tfx_wertungen.int_wertungenid LEFT JOIN tfx_jury_results AS jr ON jr.int_wertungenid = tfx_wertungen.int_wertungenid AND jr.int_kp = tfx_wertungen_details.int_kp AND jr.int_versuch = tfx_wertungen_details.int_versuch AND int_disziplinen_felderid=(SELECT int_disziplinen_felderid FROM tfx_disziplinen_felder WHERE int_disziplinenid=tfx_wettkaempfe_x_disziplinen.int_disziplinenid AND bol_ausgangswert='true' AND bol_enabled='true' LIMIT 1) WHERE tfx_wettkaempfe.int_wettkaempfeid=? AND int_runde=? GROUP BY tfx_wertungen.int_wertungenid, tfx_wettkaempfe_x_disziplinen.int_disziplinenid, tfx_wertungen_details.int_kp ORDER BY tfx_wertungen.int_wertungenid, tfx_wettkaempfe_x_disziplinen.int_disziplinenid, tfx_wertungen_details.int_kp DESC");
    res.bindValue(0,wk.value(3).toInt());
    res.bindValue(1,rnd);
    res.exec();

    QMap <int, QMap< int, QMap< int,double > > > wertungen;
    QMap< int, QMap< int,double > > wert;
    QMap <int, QMap< int, QMap< int,double > > > ausgang;
    QMap< int, QMap< int,double > > aw;

    QList<QStringList> reslist;
    int lastw = 0;
    while(res.next()) {
        if (res.at() == 0) lastw = res.value(0).toInt();
        if (res.value(0).toInt() != lastw && res.at()>0) {
            wertungen[lastw] = wert;
            ausgang[lastw] = aw;
            wert.clear();
            aw.clear();
            lastw = res.value(0).toInt();
        }
        wert[res.value(1).toInt()][res.value(4).toInt()] = res.value(2).toDouble();
        aw[res.value(1).toInt()][res.value(4).toInt()] = res.value(3).toDouble();
    }
    if (_global::querySize(res)>0) {
        wertungen[lastw] = wert;
        ausgang[lastw] = aw;
    }
    if (competition->getType() == 0 || competition->getType() == 2) {
        QString pgExtra;
        if (_global::getDBTyp()==0) {
            pgExtra = "::text";
        }
        QSqlQuery tn;
        QString tnquery;
        tnquery = "SELECT CASE WHEN bol_ak='true' THEN 'AK' ELSE int_wertungenid"+pgExtra+" END AS platz, CASE WHEN tfx_wertungen.int_gruppenid IS NULL THEN ";
        tnquery += _global::nameFormat();
        tnquery += " ELSE tfx_gruppen.var_name END"+pgExtra+" || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END AS tnname, tfx_vereine.var_name, CASE WHEN tfx_wertungen.int_gruppenid IS NULL THEN "+_global::date("dat_geburtstag",2)+" END, tfx_wertungen.int_wertungenid FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_gruppen.int_vereineid = tfx_vereine.int_vereineid OR tfx_teilnehmer.int_vereineid = tfx_vereine.int_vereineid INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) INNER JOIN tfx_laender USING (int_laenderid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND var_nummer=? AND int_runde=? AND bol_startet_nicht='false'"+detailQuery+" ORDER BY bol_ak DESC";
        tn.prepare(tnquery);
        tn.bindValue(0, competition->getEvent()->getMainEventId());
        tn.bindValue(1, competition->getNumber());
        tn.bindValue(2, rnd);
        tn.exec();
        while (tn.next()) {
            QStringList tnlist;
            for (int i=0;i<4;i++) {
                tnlist << tn.value(i).toString();
            }
            double sum=0;
            QList<double> streich;
            for (int i=0;i<disrows.size();i++) {
                dis.seek(disrows.at(i).at(0));
                double dres = wertungen[tn.value(4).toInt()][dis.value(0).toInt()][disrows.at(i).at(1)];
                if (dres > 0) {
                    double calc;
                    FunctionParser fparser;
                    fparser.Parse(dis.value(2).toString().replace(",",".").toStdString(),"x");
                    double Vars[] = {dres};
                    calc = fparser.Eval(Vars);
                    if (calc < 0) calc = 0;
                    if (dis.value(4).toDouble()>0) calc = qMin(dis.value(4).toDouble(),calc);
                    QString res = QString().setNum(calc,'f',dis.value(1).toInt());
                    QString resString = res;
                    if (printAW) {
                        double awert = ausgang[tn.value(4).toInt()][dis.value(0).toInt()][disrows.at(i).at(1)];
                        if (awert>0) {
                            resString += " (";
                            resString += QString().setNum(awert,'f',dis.value(1).toInt());
                            resString += ")";
                        }
                    }
                    tnlist << resString;
                    streich.append(res.toDouble());
                    sum += res.toDouble();
                } else {
                    tnlist << QString().setNum(wertungen[tn.value(4).toInt()][dis.value(0).toInt()][disrows.at(i).at(1)],'f',dis.value(1).toInt());
                    streich.append(0.0);
                }
            }
            if (wk.value(0).toBool() && wk.value(7).toInt() != 1) {
                std::sort(streich.begin(),streich.end());
                for (int i=0;i<wk.value(6).toInt();i++) {
                    sum -= streich.at(i);
                }
            }
            tnlist << QString().setNum(sum,'f',3);
            tnlist << tn.value(4).toString();
            reslist.append(tnlist);
        }
    } else {
        QSqlQuery team;
        team.prepare("SELECT int_mannschaftenid, tfx_vereine.var_name, tfx_mannschaften.int_nummer ||'. Mannschaft', int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) INNER JOIN tfx_laender USING (int_laenderid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_mannschaften.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=?"+detailQuery);
        team.bindValue(0, competition->getEvent()->getMainEventId());
        team.bindValue(1, competition->getNumber());
        team.exec();
        QSqlQuery tn;
        tn.prepare("SELECT int_wertungenid FROM tfx_wertungen WHERE int_mannschaftenid=? AND bol_ak='false'");
        QSqlQuery abzug;
        abzug.prepare("SELECT rel_abzug FROM tfx_mannschaften_abzug INNER JOIN tfx_man_x_man_ab USING (int_mannschaften_abzugid) WHERE int_mannschaftenid=?");
        QMap<int, QMap<int, double > > dispoints;
        while (team.next()) {
            tn.bindValue(0,team.value(3).toInt());
            tn.exec();
            int tnsize = _global::querySize(tn);
            QStringList teamlist;
            for (int i=0;i<3;i++) {
                teamlist << team.value(i).toString();
            }
            double sum=0;
            QList<double> streich;
            for (int d=0;d<disrows.size();d++) {
                dis.seek(disrows.at(d).at(0));
                QList<double> tlist;
                for (int i=0;i<tnsize;i++) {
                    tn.seek(i);
                    double dres = wertungen[tn.value(0).toInt()][dis.value(0).toInt()][disrows.at(d).at(1)];
                    if (dres > 0) {
                        double calc;
                        FunctionParser fparser;
                        fparser.Parse(dis.value(2).toString().replace(",",".").toStdString(),"x");
                        double Vars[] = {dres};
                        calc = fparser.Eval(Vars);
                        if (calc < 0) calc = 0;
                        if (dis.value(4).toDouble()>0) calc = qMin(dis.value(4).toDouble(),calc);
                        QString res = QString().setNum(calc,'f',dis.value(1).toInt());
                        tlist.append(res.toDouble());
                    } else {
                        tlist.append(0);
                    }
                }
                std::sort(tlist.begin(),tlist.end());
                double total=0;
                for (int i=(tlist.size()-1);i>(tlist.size()-1-wk.value(2).toInt());i--) {
                    if (i>=0) {
                        total += tlist.at(i);
                    }
                }
                dispoints[dis.value(0).toInt()][reslist.size()] = total;
                teamlist << QString().setNum(total,'f',dis.value(1).toInt());
                sum += total;
                streich.append(total);
            }
            abzug.bindValue(0,team.value(3).toInt());
            abzug.exec();
            while (abzug.next()) {
                sum -= abzug.value(0).toDouble();
            }
            if (wk.value(0).toBool()) {
                std::sort(streich.begin(),streich.end());
                for (int i=0;i<wk.value(6).toInt();i++) {
                    sum -= streich.at(i);
                }
            }
            teamlist << QString().setNum(sum,'f',3);
            teamlist << team.value(3).toString();
            reslist.append(teamlist);
        }
        if (wk.value(5).toBool()) {
            for (int d=0;d<disrows.size();d++) {
                dis.seek(disrows.at(d).at(0));
                double max=0;
                QList<int> maxv;
                QMapIterator<int,double> i(dispoints[dis.value(0).toInt()]);
                while (i.hasNext()) {
                    i.next();
                    if (i.value()>max) {
                        maxv.clear();
                        max = i.value();
                        maxv.append(i.key());
                    } else if (i.value() == max) {
                        maxv.append(i.key());
                    }
                }
                for (int i=0;i<maxv.size();i++) {
                    double add=2;
                    if (maxv.size()>1) add=1;
                    QStringList teamlist = reslist.at(maxv.at(i));
                    //double sum = teamlist.at(teamlist.size()-2).toDouble();
                    //sum += add;
                    teamlist.replace(3+d,teamlist.at(3+d)+" (+"+QString::number(add)+")");
                    //teamlist.replace(teamlist.size()-2,QString().setNum(sum,'f',3));
                    reslist.replace(maxv.at(i),teamlist);
                }
            }
        }
    }
    int sort;
    if ((competition->getType() == 0 || competition->getType() == 2) && wk.value(4).toBool()) {
        sort = 3;
    } else {
        sort = 0;
    }
    reslist = sortRes(reslist,sort);
    int p=0;
    double last=0;
    if (reslist.size() > 0) {
        int k = reslist.at(0).size()-2;
        for (int i=reslist.size()-1;i>=0;i--) {
            if (reslist.at(i).at(0) == "AK") {
                reslist.move(i,reslist.size()-1);
            }
        }
        for (int i=0;i<reslist.size();i++) {
            if (reslist.at(i).at(0) != "AK") {
                if (competition->getType() == 0 && wk.value(4).toBool()) {
                    if (reslist.at(i).at(k).toDouble() > last || i==0) {
                        p = i+1;
                        last = reslist.at(i).at(k).toDouble();
                    }
                } else {
                    if (reslist.at(i).at(k).toDouble() < last || i==0) {
                        p = i+1;
                        last = reslist.at(i).at(k).toDouble();
                    }
                }
                QString q = "";
                if (!competition->getEvent()->isMultiRoundEvent()) {
                    if (p <= wk.value(1).toInt()) q = " Q";
                }
                QStringList lst = reslist.at(i);
                lst.replace(0,QString().setNum(p)+q);
                reslist.replace(i,lst);
            }
        }
    }
    if (cres.length() > 0) {
        QList<int> ids;
        QSqlQuery id;
        if (competition->getType() == 0 || competition->getType() == 2) {
            QString query = "SELECT int_wertungenid FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid WHERE int_wettkaempfeid=? AND tfx_vereine.int_vereineid IN (" + _global::intListToString(cres) + ")";
            id.prepare(query);
        } else {
            QString query = "SELECT int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) WHERE int_wettkaempfeid=? AND int_vereineid IN (" + _global::intListToString(cres) + ")";
            id.prepare(query);
        }
        id.bindValue(0,wk.value(3).toInt());
        id.exec();
        while (id.next()) {
            ids.append(id.value(0).toInt());
        }
        for (int i=(reslist.size()-1);i>=0;i--) {
            if (!ids.contains(reslist.at(i).last().toInt())) {
                reslist.removeAt(i);
            }
        }
    }
    return reslist;
}

QList<QStringList> Result_Calc::roundResultArrayNew(Competition *competition,bool useExtraScore, QString detailQuery) {
    QSqlQuery wk;
    wk.prepare("SELECT bol_streichwertung, int_qualifikation, int_wertungen, int_wettkaempfeid FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND var_nummer=? LIMIT 1");
    wk.bindValue(0, competition->getEvent()->getMainEventId());
    wk.bindValue(1, competition->getNumber());
    wk.exec();
    wk.next();
    QSqlQuery rnd;
    rnd.prepare("SELECT int_runde FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? OR int_hauptwettkampf=? AND bol_rundenwettkampf='true' ORDER BY int_runde");
    rnd.bindValue(0,competition->getEvent()->getMainEventId());
    rnd.bindValue(1,competition->getEvent()->getMainEventId());
    rnd.exec();
    QMap <int, QMap< int,double > > wertungen;
    QMap <int, QMap< int,int > > places;
    QList<QStringList> reslist;
    while(rnd.next()) {
        QList<QStringList> lst = resultArrayNew(competition, QList<int>(), rnd.value(0).toInt(), 1, false, detailQuery);
        for (int i=0;i<lst.size();i++) {
            wertungen[lst.at(i).last().toInt()][rnd.value(0).toInt()] = lst.at(i).at(lst.at(i).size()-2).toDouble();
            places[lst.at(i).last().toInt()][rnd.value(0).toInt()] = lst.at(i).at(0).toInt();
        }
    }
    if (competition->getType() == 1) {
        QSqlQuery team;
        team.prepare("SELECT int_mannschaftenid, tfx_vereine.var_name, tfx_mannschaften.int_nummer ||'. Mannschaft', int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) INNER JOIN tfx_laender USING (int_laenderid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_mannschaften.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=?"+detailQuery);
        team.bindValue(0, competition->getEvent()->getMainEventId());
        team.bindValue(1, competition->getNumber());
        team.exec();
        while (team.next()) {
            QStringList teamlist;
            for (int i=0;i<3;i++) {
                teamlist << team.value(i).toString();
            }
            double sum=0;
            int ranks=0;
            int rndsize = _global::querySize(rnd);
            for (int d=0;d<rndsize;d++) {
                rnd.seek(d);
                teamlist << QString().setNum(wertungen[team.value(3).toInt()][rnd.value(0).toInt()],'f',3);
                teamlist << QString().setNum(places[team.value(3).toInt()][rnd.value(0).toInt()]);
                sum += wertungen[team.value(3).toInt()][rnd.value(0).toInt()];
                ranks += places[team.value(3).toInt()][rnd.value(0).toInt()];
            }
            teamlist << QString().setNum(sum,'f',3);
            teamlist << QString().setNum(ranks);
            teamlist << team.value(3).toString();
            reslist.append(teamlist);
        }
    } else {
        QString pgExtra;
        if (_global::getDBTyp()==0) {
            pgExtra = "::text";
        }
        QSqlQuery tn;
        QString tnquery;
        if (competition->getType() == 2) {
            tnquery = "SELECT CASE WHEN bol_ak='true' THEN 'AK' ELSE '' END AS platz, tfx_gruppen.var_name, tfx_vereine.var_name, '', tfx_gruppen.int_gruppenid FROM tfx_wertungen INNER JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_gruppen.int_vereineid = tfx_vereine.int_vereineid INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND var_nummer=? AND bol_startet_nicht='false' GROUP BY tfx_gruppen.int_gruppenid, bol_ak, tfx_gruppen.var_name, tfx_vereine.var_name ORDER BY bol_ak DESC";
        } else {
            tnquery = "SELECT CASE WHEN bol_ak='true' THEN 'AK' ELSE int_wertungenid"+pgExtra+" END AS platz, CASE WHEN tfx_wertungen.int_gruppenid IS NULL THEN ";
            tnquery += _global::nameFormat();
            tnquery += " ELSE tfx_gruppen.var_name END"+pgExtra+" || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END AS tnname, tfx_vereine.var_name, CASE WHEN tfx_wertungen.int_gruppenid IS NULL THEN "+_global::date("dat_geburtstag",2)+" END, tfx_wertungen.int_wertungenid FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_gruppen ON tfx_wertungen.int_gruppenid = tfx_gruppen.int_gruppenid INNER JOIN tfx_vereine ON tfx_gruppen.int_vereineid = tfx_vereine.int_vereineid OR tfx_teilnehmer.int_vereineid = tfx_vereine.int_vereineid INNER JOIN tfx_gaue USING (int_gaueid) INNER JOIN tfx_verbaende USING (int_verbaendeid) INNER JOIN tfx_laender USING (int_laenderid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_veranstaltungenid=? AND var_nummer=? AND bol_startet_nicht='false'"+detailQuery+" ORDER BY bol_ak DESC";
        }
        tn.prepare(tnquery);
        tn.bindValue(0, competition->getEvent()->getMainEventId());
        tn.bindValue(1, competition->getNumber());
        tn.exec();
        while (tn.next()) {
            QStringList tnlist;
            for (int i=0;i<4;i++) {
                tnlist << tn.value(i).toString();
            }
            double sum=0;
            int ranks=0;
            int rndsize = _global::querySize(rnd);
            for (int d=0;d<rndsize;d++) {
                rnd.seek(d);
                tnlist << QString().setNum(wertungen[tn.value(4).toInt()][rnd.value(0).toInt()],'f',3);
                tnlist << QString().setNum(places[tn.value(4).toInt()][rnd.value(0).toInt()]);
                sum += wertungen[tn.value(4).toInt()][rnd.value(0).toInt()];
                ranks += places[tn.value(4).toInt()][rnd.value(0).toInt()];
            }
            tnlist << QString().setNum(sum,'f',3);
            tnlist << QString().setNum(ranks);
            tnlist << tn.value(4).toString();
            reslist.append(tnlist);
        }
    }
    if (useExtraScore) {
        reslist = sortRes(reslist,2);
    } else {
        reslist = sortRes(reslist,1);
    }
    int p=0;
    double last=0;
    int last2=0;
    if (reslist.size()>0) {
        int k = reslist.at(0).size()-2;
        int l = reslist.at(0).size()-3;
        for (int i=reslist.size()-1;i>=0;i--) {
            if (reslist.at(i).at(0) == "AK") {
                reslist.move(i,reslist.size()-1);
            }
        }
        for (int i=0;i<reslist.size();i++) {
            if (reslist.at(i).at(0) != "AK") {
                if (useExtraScore) {
                    if ((reslist.at(i).at(k).toInt() > last2) || i==0 || (reslist.at(i).at(k).toInt() == last2 && reslist.at(i).at(l).toDouble() != last)) {
                        p = i+1;
                        last = reslist.at(i).at(l).toDouble();
                        last2 = reslist.at(i).at(k).toInt();
                    }
                } else {
                    if (reslist.at(i).at(l).toDouble() < last || i==0) {
                        p = i+1;
                        last = reslist.at(i).at(l).toDouble();
                    }
                }
                QString q = "";
                if (p <= wk.value(1).toInt()) q = " Q";
                QStringList lst = reslist.at(i);
                lst.replace(0,QString().setNum(p)+q);
                reslist.replace(i,lst);
            }
        }
    }
    return reslist;
}

QList<QStringList> Result_Calc::tabllenArray(Competition *competition) {
    QSqlQuery rnd;
    rnd.prepare("SELECT int_runde FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? OR int_hauptwettkampf=? ORDER BY int_runde");
    rnd.bindValue(0, competition->getEvent()->getMainEventId());
    rnd.bindValue(1, competition->getEvent()->getMainEventId());
    rnd.exec();

    QMap <int, QMap< int,double > > wertungen;
    QMap <int, QMap< int,int > > point1;
    QMap <int, QMap< int,int > > point2;

    QMap< int,double > roundSum;

    while(rnd.next()) {
        QList<QStringList> lst = resultArrayNew(competition,QList<int>(),rnd.value(0).toInt());
        for (int i=0;i<lst.size();i++) {
            wertungen[lst.at(i).last().toInt()][rnd.value(0).toInt()] = lst.at(i).at(lst.at(i).size()-2).toDouble();
            roundSum[rnd.value(0).toInt()] += lst.at(i).at(lst.at(i).size()-2).toDouble();
            point1[lst.at(i).last().toInt()][rnd.value(0).toInt()] = 0;
            point2[lst.at(i).last().toInt()][rnd.value(0).toInt()] = 0;
        }
    }

    QMap <int, QMap< int,double > > wertungen2 = wertungen;

    QMapIterator<int, QMap< int,double > > i(wertungen);

    QList<int> processedTeams;

    while (i.hasNext()) {
        i.next();
        QMapIterator<int, QMap< int,double > > j(wertungen2);
        while (j.hasNext()) {
            j.next();
            if (j.key() == i.key()) continue;
            if (processedTeams.contains(j.key())) continue;
            QMapIterator<int,double> k(i.value());
            while (k.hasNext()) {
                k.next();
                if (roundSum[k.key()]<=0) continue;
                if (wertungen[i.key()][k.key()] > wertungen[j.key()][k.key()]) {
                    point1[i.key()][k.key()] += 2;
                    point2[j.key()][k.key()] += 2;
                } else if (wertungen[i.key()][k.key()] < wertungen[j.key()][k.key()]) {
                    point1[j.key()][k.key()] += 2;
                    point2[i.key()][k.key()] += 2;
                } else {
                    point1[i.key()][k.key()]++;
                    point2[i.key()][k.key()]++;
                    point1[j.key()][k.key()]++;
                    point2[j.key()][k.key()]++;
                }
            }
        }
        processedTeams.append(i.key());
    }

    QList<QStringList> reslist;

    QSqlQuery team;
    team.prepare("SELECT int_mannschaftenid, tfx_vereine.var_name, tfx_mannschaften.int_nummer ||'. Mannschaft', int_mannschaftenid FROM tfx_mannschaften INNER JOIN tfx_vereine USING (int_vereineid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_mannschaften.int_wettkaempfeid WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=?");
    team.bindValue(0, competition->getEvent()->getMainEventId());
    team.bindValue(1, competition->getNumber());
    team.exec();
    while (team.next()) {
        QStringList teamlist;
        for (int i=0;i<3;i++) {
            teamlist << team.value(i).toString();
        }
        double sum=0;
        int pts1=0;
        int pts2=0;
        int rndsize = _global::querySize(rnd);
        for (int d=0;d<rndsize;d++) {
            rnd.seek(d);
            teamlist << QString().setNum(wertungen[team.value(3).toInt()][rnd.value(0).toInt()],'f',3);
            teamlist << QString().setNum(point1[team.value(3).toInt()][rnd.value(0).toInt()])+" : "+QString().setNum(point2[team.value(3).toInt()][rnd.value(0).toInt()]);
            sum += wertungen[team.value(3).toInt()][rnd.value(0).toInt()];
            pts1 += point1[team.value(3).toInt()][rnd.value(0).toInt()];
            pts2 += point2[team.value(3).toInt()][rnd.value(0).toInt()];
        }
        teamlist << QString().setNum(pts1);
        teamlist << QString().setNum(pts2);
        teamlist << QString().setNum(sum,'f',3);
        teamlist << team.value(3).toString();
        reslist.append(teamlist);
    }

    int k = reslist.at(0).size()-4;
    int m = reslist.at(0).size()-3;
    int n = reslist.at(0).size()-2;
    for (int i=0;i<reslist.size();i++) {
        for (int j=0;j<(i+1);j++) {
            if (reslist.at(i).at(k).toInt() > reslist.at(j).at(k).toInt()) {
                reslist.swapItemsAt(i,j);
            } else if (reslist.at(i).at(k).toInt() == reslist.at(j).at(k).toInt()) {
                if (reslist.at(i).at(m).toInt() < reslist.at(j).at(m).toInt()) {
                    reslist.swapItemsAt(i,j);
                } else if (reslist.at(i).at(m).toInt() == reslist.at(j).at(m).toInt()) {
                    if (reslist.at(i).at(n).toDouble() > reslist.at(j).at(n).toDouble()) {
                        reslist.swapItemsAt(i,j);
                    }
                }
            }
        }
    }
    double last=0;
    int p=0;
    for (int i=0;i<reslist.size();i++) {
        if (reslist.at(i).at(n).toDouble() < last || i==0) {
            p = i+1;
            last = reslist.at(i).at(n).toDouble();
        }
        QStringList lst = reslist.at(i);
        lst.replace(0,QString().setNum(p));
        reslist.replace(i,lst);
    }
    return reslist;
}


QList<QStringList> Result_Calc::sortRes(QList<QStringList> l,int mode) {
    if (l.size() > 0) {
        if (mode==2) l = quicksort(l,0,l.size()-1,1);
        l = quicksort(l,0,l.size()-1,mode);
    }
    return l;
}

QList<QStringList> Result_Calc::quicksort(QList<QStringList> a, int left, int right, int mode) {

    int k = a.at(0).size()-2;
    int m = a.at(0).size()-3;
    if (mode == 1) k = m; //Rundenwettkampf

    if (left < right) {

        double pivot = a.at(right).at(k).toDouble();
        int l = left;
        int r = right;

        do {
            if (mode == 0 || mode == 1) {
                while (a.at(l).at(k).toDouble() > pivot) l++;
                while (a.at(r).at(k).toDouble() < pivot) r--;
            } else {
                while (a.at(l).at(k).toDouble() < pivot) l++;
                while (a.at(r).at(k).toDouble() > pivot) r--;
            }
            if (l <= r) {
                if (mode == 2 && a.at(l).at(k).toDouble() == a.at(r).at(k).toDouble() && a.at(l).at(m).toDouble() > a.at(r).at(m).toDouble()) {
                    l++;
                    r--;
                    continue;
                }
                a.swapItemsAt(l,r);
                l++;
                r--;
            }
        } while (l <= r);

        a = quicksort(a, left, r, mode);
        a = quicksort(a, l, right, mode);
    }
    return a;
}
