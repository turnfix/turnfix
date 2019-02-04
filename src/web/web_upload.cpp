#include <QFile>
#include <QSqlRecord>
#include <QVariant>
#include <QDomDocument>
#include "header/web_upload.h"
#include "../global/header/_global.h"
#include "../global/header/settings.h"
#include "../export/ergebnisse/header/runde.h"
#include "../export/ergebnisse/header/detail.h"
#include "../export/ergebnisse/header/ergebnisse.h"
#include "../global/header/result_calc.h"

void TOA_Upload::run() {
    Ergebnisse::setCoverID(0);
    Ergebnisse::setHeadFootID(headFoot);
    Detail::setPrintAW(awExtra);
    createPDF();
    quit();
}

void TOA_Upload::setRndRes(bool rnd) {
    rndres = rnd;
}

void TOA_Upload::setDetailInfo(int di) {
    detailinfo = di;
}

void TOA_Upload::setHeadFootID(int set) {
    headFoot = set;
}

void TOA_Upload::setAWExtra(bool set) {
    awExtra = set;
}

void TOA_Upload::setLandscape(bool set) {
    landscape = set;
}

void TOA_Upload::createPDF() {
    Ergebnisse *ausdruck;
    Ergebnisse::setDetailInfo(detailinfo);
    QSqlQuery query;
    query.prepare("SELECT var_nummer FROM tfx_wettkaempfe WHERE int_veranstaltungenid=?");
    query.bindValue(0, _global::checkHWK());
    query.exec();
    int total = _global::querySize(query)+2;
    int i=1;
    files.append("XML");
    files.append("gesamt.pdf");
    emit(textChanged("PDF-Dateien werden erzeugt...\ngesamt.pdf"));
    emit(numberChanged(0,total));
    if (rndres) {
        ausdruck = new Runde;
    } else {
        ausdruck = new Detail;
    }
    ausdruck->setOutputType(2);
    ausdruck->setOutputFileName("gesamt.pdf");
    ausdruck->setTypeString("Ergebnisse");
    if (landscape) {
        ausdruck->setOrientation(QPrinter::Landscape);
    } else {
        ausdruck->setOrientation(QPrinter::Portrait);
    }
    ausdruck->start();
    ausdruck->wait();
    emit(numberChanged(i,total));
    while (query.next()) {
        files.append(query.value(0).toString() + ".pdf");
        emit(textChanged("PDF-Dateien werden erzeugt...\n" + query.value(0).toString() + ".pdf"));
        if (rndres) {
            ausdruck = new Runde;
        } else {
            ausdruck = new Detail;
        }
        ausdruck->setOutputType(2);
        ausdruck->setOutputFileName(query.value(0).toString() + ".pdf");
        ausdruck->setTypeString("Ergebnisse");
        if (landscape) {
            ausdruck->setOrientation(QPrinter::Landscape);
        } else {
            ausdruck->setOrientation(QPrinter::Portrait);
        }
        ausdruck->setWKNumber(query.value(0).toString());
        ausdruck->start();
        ausdruck->wait();
        i++;
        emit(numberChanged(i,total));
    }
    query.clear();
    QString xml = createXML().toString();
    emit(numberChanged(0,0));
    emit uploadFiles(files,xml);
}

QDomDocument TOA_Upload::createXML() {
    emit(textChanged("XML Datei wird erzeugt..."));
    QSqlQuery query;
    query.prepare("SELECT * FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=?");
    query.bindValue(0,_global::getWkNr());
    query.exec();
    query.next();
    QDomDocument doc;
    QDomElement root = doc.createElement("Ergebnisse");
    doc.appendChild(root);
    QDomElement de_name = doc.createElement("WKName");
    root.appendChild(de_name);
    QDomText dt_name = doc.createTextNode(query.value(6).toString());
    de_name.appendChild(dt_name);
    QDomElement de_rnd = doc.createElement("RndRes");
    root.appendChild(de_rnd);
    if (rndres) {
        QDomText dt_rnd = doc.createTextNode("true");
        de_rnd.appendChild(dt_rnd);
    } else {
        QDomText dt_rnd = doc.createTextNode("false");
        de_rnd.appendChild(dt_rnd);
    }
    QDomElement de_dstart = doc.createElement("StartDatum");
    root.appendChild(de_dstart);
    QDomText dt_dstart = doc.createTextNode(query.value(8).toString());
    de_dstart.appendChild(dt_dstart);
    QDomElement de_dende = doc.createElement("EndeDatum");
    root.appendChild(de_dende);
    QDomText dt_dende = doc.createTextNode(query.value(9).toString());
    de_dende.appendChild(dt_dende);
    QDomElement de_veranstalter = doc.createElement("Veranstalter");
    root.appendChild(de_veranstalter);
    QDomText dt_veranstalter = doc.createTextNode(query.value(12).toString());
    de_veranstalter.appendChild(dt_veranstalter);
    QDomElement de_ort = doc.createElement("Ort");
    root.appendChild(de_ort);
    QDomText dt_ort = doc.createTextNode(query.value(29).toString() + " " + query.value(32).toString());
    de_ort.appendChild(dt_ort);
    QString swknr="";
    int wahlwk=0;
    int count = 0;
    QSqlQuery counttn;
    counttn.prepare("SELECT COUNT(*) FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND int_runde=?");
    counttn.bindValue(0,_global::checkHWK());
    counttn.bindValue(1, _global::getRunde());
    counttn.exec();
    counttn.next();
    QSqlQuery query5;
    query5.prepare("SELECT var_nummer, bol_wahlwettkampf, var_name, bol_ak_anzeigen, yer_von, yer_bis, int_typ FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? AND (SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid) > 0 ORDER BY var_nummer");
    query5.bindValue(0,_global::checkHWK());
    query5.exec();
    while (query5.next()) {
        QDomElement de_wkdata = doc.createElement("Wettkampf");
        root.appendChild(de_wkdata);
        QDomElement de_wknr = doc.createElement("WKNr");
        de_wkdata.appendChild(de_wknr);
        QDomText dt_wknr = doc.createTextNode(query5.value(0).toString());
        de_wknr.appendChild(dt_wknr);
        QDomElement de_wahl = doc.createElement("WahlWK");
        de_wkdata.appendChild(de_wahl);
        QDomText dt_wahl = doc.createTextNode(query5.value(1).toString());
        de_wahl.appendChild(dt_wahl);
        QDomElement de_wkbez = doc.createElement("WKBez");
        de_wkdata.appendChild(de_wkbez);
        QDomText dt_wkbez = doc.createTextNode(query5.value(2).toString());
        de_wkbez.appendChild(dt_wkbez);
        QDomElement de_typ = doc.createElement("WKTyp");
        de_wkdata.appendChild(de_typ);
        QDomText dt_typ = doc.createTextNode(query5.value(6).toString());
        de_typ.appendChild(dt_typ);
        QDomElement de_jahrgang = doc.createElement("Jahrgang");
        de_wkdata.appendChild(de_jahrgang);
        QDomText dt_jahrgang = doc.createTextNode(_global::wkBez(query5.value(0).toString()));
        de_jahrgang.appendChild(dt_jahrgang);
        swknr = query5.value(0).toString();
        wahlwk = query5.value(1).toInt();
        QList<QStringList> tnarray;
        if (rndres) {
            tnarray = Result_Calc::roundResultArrayNew(swknr);
        } else {
            tnarray = Result_Calc::resultArrayNew(swknr);
        }
        for (int i=0;i<tnarray.size();i++) {
            if (_global::checkTyp(swknr)!=1 || rndres) {
                de_wkdata.appendChild(createStarterElement(swknr, tnarray.at(i).last().toInt(),doc,tnarray));
                count++;
            } else {
                QDomElement de_starter = doc.createElement("Starter");
                de_wkdata.appendChild(de_starter);
                QDomElement de_platz = doc.createElement("Platz");
                de_starter.appendChild(de_platz);
                QDomText dt_platz = doc.createTextNode(tnarray.at(i).at(0));
                de_platz.appendChild(dt_platz);
                QDomElement de_tnname = doc.createElement("TNName");
                de_starter.appendChild(de_tnname);
                QDomText dt_tnname = doc.createTextNode(tnarray.at(i).at(1));
                de_tnname.appendChild(dt_tnname);
                QDomElement de_tnverein = doc.createElement("TNVerein");
                de_starter.appendChild(de_tnverein);
                QDomText dt_tnverein = doc.createTextNode(tnarray.at(i).at(2));
                de_tnverein.appendChild(dt_tnverein);
                QDomElement de_tnpkt = doc.createElement("TNPunkte");
                de_starter.appendChild(de_tnpkt);
                QDomText dt_tnpkt = doc.createTextNode(tnarray.at(i).at(tnarray.at(i).size()-2));
                de_tnpkt.appendChild(dt_tnpkt);
                QSqlQuery dis;
                dis.prepare("SELECT var_kurz2 FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung");
                dis.bindValue(0,_global::checkHWK());
                dis.bindValue(1,swknr);
                dis.exec();
                while (dis.next()) {
                    QDomElement de_wertungen = doc.createElement("Wertungen");
                    QDomElement de_wertname = doc.createElement("Name");
                    de_wertungen.appendChild(de_wertname);
                    QDomText dt_wertname = doc.createTextNode(dis.value(0).toString());
                    de_wertname.appendChild(dt_wertname);
                    QDomElement de_points = doc.createElement("Punkte");
                    de_wertungen.appendChild(de_points);
                    QDomText dt_points = doc.createTextNode(tnarray.at(i).at(dis.at()+3));
                    de_points.appendChild(dt_points);
                    QDomElement de_wert = doc.createElement("Wert");
                    de_wertungen.appendChild(de_wert);
                    de_starter.appendChild(de_wertungen);
                }
                QList<QStringList> teamarray = Result_Calc::resultArrayNew(swknr,0);
                QSqlQuery dres;
                dres.prepare("SELECT tfx_wertungen.int_wertungenid FROM tfx_wertungen INNER JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid WHERE tfx_wertungen.int_mannschaftenid=? AND tfx_wertungen.int_runde=? ORDER BY bol_ak, var_nachname, var_vorname");
                dres.bindValue(0,tnarray.at(i).last());
                dres.bindValue(1,_global::getRunde());
                dres.exec();
                while (dres.next()) {
                    de_starter.appendChild(createStarterElement(swknr, dres.value(0).toInt(),doc, teamarray));
                }
                QSqlQuery abzugq;
                abzugq.prepare("SELECT rel_abzug, tfx_mannschaften_abzug.var_name AS tabzug FROM tfx_man_x_man_ab INNER JOIN tfx_mannschaften_abzug USING (int_mannschaften_abzugid) WHERE tfx_man_x_man_ab.int_mannschaftenid=?");
                abzugq.bindValue(0,tnarray.at(i).last());
                abzugq.exec();
                while (abzugq.next()) {
                    QDomElement de_tabz = doc.createElement("TeamAbzug");
                    de_starter.appendChild(de_tabz);
                    QDomText dt_tabz = doc.createTextNode(QString("*** " + abzugq.value(0).toString() + " Pkt. Abzug wegen " + abzugq.value(1).toString() + " ***"));
                    de_tabz.appendChild(dt_tabz);
                }
                count++;
            }
        }
        QDomElement de_count = doc.createElement("TNCount");
        de_wkdata.appendChild(de_count);
        QDomText dt_count = doc.createTextNode(QString::number(count));
        de_count.appendChild(dt_count);
    }
    query.clear();
    return doc;
}

QDomElement TOA_Upload::createStarterElement(QString swknr, int id, QDomDocument doc, QList<QStringList> rlist) {
    QDomElement de_starter;
    for (int i=0;i<rlist.size();i++) {
        if (rlist.at(i).last().toInt() == id) {
            de_starter = doc.createElement("Starter");
            if (_global::checkTyp(swknr) == 0 || rndres) {
                QDomElement de_platz = doc.createElement("Platz");
                de_starter.appendChild(de_platz);
                QDomText dt_platz = doc.createTextNode(rlist.at(i).at(0));
                de_platz.appendChild(dt_platz);
            }
            QDomElement de_tnname = doc.createElement("TNName");
            de_starter.appendChild(de_tnname);
            QDomText dt_tnname = doc.createTextNode(rlist.at(i).at(1));
            de_tnname.appendChild(dt_tnname);
            if (!rndres) {
                QDomElement de_tnjg = doc.createElement("TNJahrgang");
                de_starter.appendChild(de_tnjg);
                QDomText dt_tnjg = doc.createTextNode(rlist.at(i).at(3));
                de_tnjg.appendChild(dt_tnjg);
            }
            QDomElement de_tnverein = doc.createElement("TNVerein");
            de_starter.appendChild(de_tnverein);
            QDomText dt_tnverein = doc.createTextNode(rlist.at(i).at(2));
            de_tnverein.appendChild(dt_tnverein);
            QDomElement de_tnpkt = doc.createElement("TNPunkte");
            de_starter.appendChild(de_tnpkt);
            QDomText dt_tnpkt;
            if (rndres) {
                dt_tnpkt = doc.createTextNode(QString().setNum(rlist.at(i).at(rlist.at(i).size()-3).toDouble(),'f',3));
            } else {
                dt_tnpkt = doc.createTextNode(QString().setNum(rlist.at(i).at(rlist.at(i).size()-2).toDouble(),'f',3));
            }
            de_tnpkt.appendChild(dt_tnpkt);
            if (rndres) {
                int csize = rlist.at(i).size()-5;
                for (int j=0;j<csize-2;j=j+2) {
                    QDomElement de_wertungen = doc.createElement("Wertungen");
                    QDomElement de_wertname = doc.createElement("Name");
                    de_wertungen.appendChild(de_wertname);
                    QDomText dt_wertname = doc.createTextNode("Runde " + QString().setNum((int)((j+1)/2)+1));
                    de_wertname.appendChild(dt_wertname);
                    QDomElement de_points = doc.createElement("Punkte");
                    de_wertungen.appendChild(de_points);
                    QDomText dt_points = doc.createTextNode(rlist.at(i).at(j+3));
                    de_points.appendChild(dt_points);
                    QDomElement de_wert = doc.createElement("Wert");
                    de_wertungen.appendChild(de_wert);
                    de_starter.appendChild(de_wertungen);
                }
            } else {
                QSqlQuery cdis;
                cdis.prepare("SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=?");
                cdis.bindValue(0, rlist.at(i).last());
                cdis.exec();
                cdis.next();
                QSqlQuery discount;
                discount.prepare("SELECT int_disziplinenid, var_einheit, var_maske, int_berechnung, var_kurz2 FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE var_nummer=? AND int_veranstaltungenid=? ORDER BY int_sortierung");
                discount.bindValue(1,_global::checkHWK());
                discount.bindValue(0,swknr);
                discount.exec();
                while (discount.next()) {
                    QSqlQuery tndis;
                    tndis.prepare("SELECT * FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=? AND int_disziplinenid=?");
                    tndis.bindValue(0, rlist.at(i).last());
                    tndis.bindValue(1, discount.value(0).toString());
                    tndis.exec();
                    tndis.next();
                    if (cdis.value(0).toInt() == 0 || (cdis.value(0).toInt() > 0 && tndis.value(0).toInt() > 0)) {
                        QDomElement de_wertungen = doc.createElement("Wertungen");
                        QDomElement de_wertname = doc.createElement("Name");
                        de_wertungen.appendChild(de_wertname);
                        QDomText dt_wertname = doc.createTextNode(discount.value(4).toString());
                        de_wertname.appendChild(dt_wertname);
                        QDomElement de_points = doc.createElement("Punkte");
                        de_wertungen.appendChild(de_points);
                        QDomText dt_points = doc.createTextNode(rlist.at(i).at(discount.at()+4));
                        de_points.appendChild(dt_points);
                        QDomElement de_wert = doc.createElement("Wert");
                        de_wertungen.appendChild(de_wert);
                        QSqlQuery query4;
                        query4.prepare("SELECT rel_leistung FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? ORDER BY rel_leistung DESC LIMIT 1");
                        query4.bindValue(0,rlist.at(i).last());
                        query4.bindValue(1,discount.value(0).toString());
                        query4.exec();
                        if (_global::querySize(query4)>0) {
                            query4.next();
                            QString leistung = _global::strLeistung(query4.value(0).toDouble(),discount.value(1).toString(),discount.value(2).toString(),discount.value(3).toInt());
                            if (leistung.startsWith("0")) leistung = leistung.right(leistung.length()-1);
                            QDomText dt_wert = doc.createTextNode(leistung);
                            de_wert.appendChild(dt_wert);
                        } else {
                            QDomText dt_wert = doc.createTextNode("0");
                            de_wert.appendChild(dt_wert);
                        }
                        de_starter.appendChild(de_wertungen);
                    }
                }
            }
            break;
        }
    }
    return de_starter;
}
