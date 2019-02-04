#include "header/detail.h"
#include "../../global/header/_global.h"
#include "../../global/header/settings.h"
#include "../../global/header/result_calc.h"

#include <QDebug>

bool Detail::printAW = false;

void Detail::setPrintAW(bool set) {
    printAW = set;
}

void Detail::printContent() {
    int fieldWidth;
    if (printAW) {
        fieldWidth = mmToPixel(26.0);
    } else {
        fieldWidth = mmToPixel(22.0);
    }
    for (int i=0;i<wkNumbers.size();i++) {
        currWK = wkNumbers.at(i);
        currWahl = wkWahl.at(i);
        currKP = wkKP.at(i);
        if (newPageWK && i > 0) {
            newPage();
        }
        if (checkFitPage(mmToPixel(55.0),currWK)) {
            printDescriptor(currWK);
        }


        //Array mit Ergebnissen erstellen
        QList<QStringList> rlist = Result_Calc::resultArrayNew(currWK,_global::checkTyp(currWK),vereinNumbers,-1,0,printAW,detailQuery);
        int cskip=0;

        //Liste mit Einzelergebnissen bei Mannschaftswettkämpfen
        QList<QStringList> dlist;
        if (_global::checkTyp(currWK) == 1) dlist = Result_Calc::resultArrayNew(currWK,0,QList<int>(),-1,0,printAW,detailQuery);

        //Ausgabe
        for (int ll=0;ll<rlist.size();ll++) {
            int skip=mmToPixel(10.0);

            if (_global::checkTyp(currWK)==1) {
                //Mannschaftswettkampf
                QSqlQuery query4;
                query4.prepare("SELECT int_wertungenid FROM tfx_wertungen WHERE int_mannschaftenid=? AND int_runde=?");
                query4.bindValue(0,rlist.at(ll).last());
                query4.bindValue(1,_global::getRunde());
                query4.exec();
                skip=skip+(_global::querySize(query4))*mmToPixel(5.8);
                if (currKP) skip += (_global::querySize(query4))*mmToPixel(5.8);
            } else {
                //Einzelwettkampf
                if (currWahl) skip=skip+mmToPixel(4.2);
                skip=skip+mmToPixel(4.2);
                cskip = 0;
            }

            //Überprüfen ob neue Seite Angefangen werden muss
            checkFitPage(skip,currWK,true);

            //Wenn Mannschafts erste Zeile Fett
            if (_global::checkTyp(currWK)==1) {
                setPrinterFont(9,true);
            } else {
                setPrinterFont(9);
            }

            //Disziplinen
            QSqlQuery query3;
            query3.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_name, int_berechnung, var_einheit, var_kurz1, var_maske, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp, bol_mansort, int_wettkaempfe_x_disziplinenid FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE var_nummer=? AND int_veranstaltungenid=? ORDER BY int_sortierung");
            query3.bindValue(0,currWK);
            query3.bindValue(1,_global::checkHWK());
            query3.exec();


            //Gerade Zeilen hervorheben
            if (ll%2 != 0) drawHighlightRect(yco);
            QString jg;
            if (_global::checkTyp(currWK)==0) jg = rlist.at(ll).at(3);
            if (_global::checkTyp(currWK)==1) {
                QList<position> dispos;
                QList<int> diskp;
                int maxx=0;
                int maxy=0;
                int col=0;
                while (query3.next()) {
                    diskp.append(query3.value(6).toInt());
                    for (int kp=0;kp<=query3.value(6).toInt();kp++) {
                        int currcol = col;
                        int currrow = 0;
                        if (query3.value(7).toBool()) {
                            QSqlQuery posquery;
                            posquery.prepare("SELECT int_sortx, int_sorty FROM tfx_wettkaempfe_dispos WHERE int_wettkaempfe_x_disziplinenid=? AND int_kp=?");
                            posquery.bindValue(0,query3.value(8).toInt());
                            posquery.bindValue(1,kp);
                            posquery.exec();
                            posquery.next();
                            currcol = posquery.value(0).toInt();
                            currrow = posquery.value(1).toInt();
                        }
                        maxx = qMax(maxx,currcol);
                        maxy = qMax(maxy,currrow);
                        position pos;
                        pos.x = currcol;
                        pos.y = currrow;
                        dispos.append(pos);
                        col++;
                    }
                }
                drawStandardRow(rlist.at(ll).at(0) + "  ",rlist.at(ll).at(1),jg,rlist.at(ll).at(2),"",readDetailInfo(false,rlist.at(ll).at(1)));
                int size = _global::querySize(query3);
                QSqlQuery dres;
                dres.prepare("SELECT tfx_wertungen.int_wertungenid FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_mannschaftenid=? AND int_runde=? AND bol_startet_nicht='false' ORDER BY bol_ak, var_nachname, var_vorname");
                dres.bindValue(0,rlist.at(ll).last());
                dres.bindValue(1,_global::getRunde());
                dres.exec();
                while (dres.next()) {
                    int j=0;
                    for (int i=0;i<dlist.size();i++) {
                        if (dlist.at(i).last().toInt() == dres.value(0).toInt()) {
                            j = i;
                            break;
                        }
                    }
                    if (ll%2 != 0) drawHighlightRect(yco);
                    int height = fontHeight;
                    setPrinterFont(6);
                    QString platz;
                    if (dlist.at(j).at(0).right(2) == " Q") {
                        platz = dlist.at(j).at(0).left(dlist.at(j).at(0).length()-2);
                    } else {
                        platz = dlist.at(j).at(0);
                    }
                    painter.drawText(QRectF(pr.x(), yco, mmToPixel(10.6), height),"(" + platz + ")  ",QTextOption(Qt::AlignVCenter | Qt::AlignRight));
                    setPrinterFont(9);
                    painter.drawText(QRectF(pr.x()+mmToPixel(10.6), yco, mmToPixel(79.4), fontHeight),dlist.at(j).at(1)+" ("+dlist.at(j).at(3)+")",QTextOption(Qt::AlignVCenter));
                    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(15.9)), yco, mmToPixel(15.9), fontHeight),dlist.at(j).at(dlist.at(j).size()-2),QTextOption(Qt::AlignVCenter | Qt::AlignRight));
                    int col=0;
                    QList<int> highlighted;
                    for (int dd=0;dd<size;dd++) {
                        for (int kp=0;kp<=diskp.at(dd);kp++) {
                            if (ll%2 != 0 && !highlighted.contains(dispos.at(col).y) && dispos.at(col).y>0) {
                                drawHighlightRect(yco+fontHeight*dispos.at(col).y);
                                highlighted.append(dispos.at(col).y);
                            }
                            painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(13.0)-((maxx+1)*fieldWidth)-((maxx+1)*mmToPixel(1.1))+(maxx-dispos.at(col).x)*fieldWidth+(maxx-dispos.at(col).x)*mmToPixel(1.1), yco+fontHeight*dispos.at(col).y, fieldWidth, fontHeight),dlist.at(j).at(4+col),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                            col++;
                        }
                    }
                    yco += fontHeight*(maxy+1);
                }
                int lineyco = yco;
                col=0;
                QList<int> highlighted;
                for (int dd=0;dd<size;dd++) {
                    for (int kp=0;kp<=diskp.at(dd);kp++) {
                        if (ll%2 != 0 && !highlighted.contains(dispos.at(col).y)) {
                            drawHighlightRect(yco+fontHeight*dispos.at(col).y);
                            highlighted.append(dispos.at(col).y);
                        }
                        painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(13.0)-((maxx+1)*fieldWidth)-((maxx+1)*mmToPixel(1.1))+(maxx-dispos.at(col).x)*fieldWidth+(maxx-dispos.at(col).x)*mmToPixel(1.1), yco+fontHeight*dispos.at(col).y, fieldWidth, fontHeight),rlist.at(ll).at(3+col),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                        col++;
                    }
                }
                painter.drawLine(QPointF(pr.width()-pr.x()-mmToPixel(13.0)-(maxx+1)*fieldWidth-(maxx+1)*mmToPixel(1.1),lineyco),QPointF(pr.width()-pr.x(),lineyco));
                setPrinterFont(9,true);
                painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(15.0), yco, mmToPixel(15.0), fontHeight),rlist.at(ll).at(rlist.at(ll).size()-2),QTextOption(Qt::AlignVCenter | Qt::AlignRight));
                yco += fontHeight*(maxy+1);
                QSqlQuery abzugq;
                abzugq.prepare("SELECT rel_abzug, var_name AS tabzug FROM tfx_mannschaften_abzug INNER JOIN tfx_man_x_man_ab USING (int_mannschaften_abzugid) WHERE int_mannschaftenid=?");
                abzugq.bindValue(0,rlist.at(ll).last());
                abzugq.exec();
                while (abzugq.next()) {
                    setPrinterFont(7,true);
                    if (ll%2 != 0) drawHighlightRect(mmToPixel(2.9));
                    painter.drawText(QRectF(pr.x(),yco,(pr.width()-pr.x()-pr.x()), fontHeight),QString("*** " + abzugq.value(0).toString() + " Pkt. Abzug wegen " + abzugq.value(1).toString() + " ***") ,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                    setPrinterFont(7);
                    yco += fontHeight;
                }
                setPrinterFont(9);
            } else {
                QString res;
                if (_global::querySize(query3) == 1) {
                    query3.next();
                    if (query3.value(6).toInt()==0) {
                        QSqlQuery query4;
                        query4.prepare("SELECT rel_leistung FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? ORDER BY rel_leistung DESC LIMIT 1");
                        query4.bindValue(0,rlist.at(ll).last());
                        query4.bindValue(1,query3.value(0).toInt());
                        query4.exec();
                        query4.next();
                        res = _global::strLeistung(query4.value(0).toDouble(),query3.value(3).toString(),query3.value(5).toString(),query3.value(2).toInt())+query3.value(3).toString();
                    } else {
                        res = rlist.at(ll).at(rlist.at(ll).size()-2);
                    }
                } else {
                    res = rlist.at(ll).at(rlist.at(ll).size()-2);
                }
                drawStandardRow(rlist.at(ll).at(0) + "  ",rlist.at(ll).at(1),jg,rlist.at(ll).at(2),res,readDetailInfo(false,rlist.at(ll).at(2)));
                if (_global::querySize(query3)>1) {
                    results.clear();
                    int maxx=0;
                    int maxy=0;
                    int col=0;
                    int at=0;
                    while (query3.next()) {
                        for (int kp=0;kp<=query3.value(6).toInt();kp++) {
                            int currcol = col;
                            int currrow = 0;
                            if (query3.value(7).toBool()) {
                                QSqlQuery posquery;
                                posquery.prepare("SELECT int_sortx, int_sorty FROM tfx_wettkaempfe_dispos WHERE int_wettkaempfe_x_disziplinenid=? AND int_kp=?");
                                posquery.bindValue(0,query3.value(8).toInt());
                                posquery.bindValue(1,kp);
                                posquery.exec();
                                posquery.next();
                                currcol = posquery.value(0).toInt();
                                currrow = posquery.value(1).toInt();
                            }
                            bool wahldis=true;
                            if (currWahl) {
                                QSqlQuery checkdis;
                                checkdis.prepare("SELECT CASE WHEN (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen  WHERE int_wertungenid=?)=0 THEN 'true' ELSE CASE WHEN (SELECT COUNT(*) FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=? AND int_disziplinenid=?)>0 THEN 'true' ELSE 'false' END END");
                                checkdis.bindValue(0,rlist.at(ll).last());
                                checkdis.bindValue(1,rlist.at(ll).last());
                                checkdis.bindValue(2,query3.value(0).toInt());
                                checkdis.exec();
                                checkdis.next();
                                wahldis = checkdis.value(0).toBool();
                            }
                            if (wahldis) {
                                resultData singleData;
                                singleData.disname = query3.value(4).toString();
                                QSqlQuery query4;
                                query4.prepare("SELECT rel_leistung FROM tfx_wertungen_details WHERE int_wertungenid=? AND int_disziplinenid=? AND int_kp=? ORDER BY rel_leistung DESC LIMIT 1");
                                query4.bindValue(0,rlist.at(ll).last());
                                query4.bindValue(1,query3.value(0).toString());
                                query4.bindValue(2,kp);
                                query4.exec();
                                QString wertung;
                                if (_global::querySize(query4)>0) {
                                    query4.next();
                                    wertung = rlist.at(ll).at(4+at);
                                } else {
                                    wertung = QString().setNum(0.0,'f',query3.value(2).toInt());
                                }
                                maxx = qMax(maxx,currcol);
                                maxy = qMax(maxy,currrow);
                                singleData.result = wertung;
                                if (_global::querySize(query4)>0) {
                                    query4.exec();
                                    query4.next();
                                    if (rlist.at(ll).at(4+at).toDouble() != query4.value(0).toDouble() && printAW == false) {
                                        QString leistung = _global::strLeistung(query4.value(0).toDouble(),query3.value(3).toString(),query3.value(5).toString(),query3.value(2).toInt());
                                        singleData.detail = leistung;
                                    }
                                }
                                QMap<int,resultData> rowMap = results[currrow];
                                rowMap.insert(currcol,singleData);
                                results.insert(currrow,rowMap);
                                col++;
                            }
                            at++;
                        }
                    }
                    setPrinterFont(9);
                    int nextyco=yco;
                    for (int i=0;i<=maxy;i++) {
                        yco = nextyco;
                        for (int j=0;j<=maxx;j++) {
                            int currYco=0;
                            if (currWahl) {
                                setPrinterFont(7,true);
                                if (ll%2 != 0 && j==0) drawHighlightRect(yco);
                                painter.drawText(QRectF((pr.x()+mmToPixel(10.6)+j*fieldWidth+j*mmToPixel(1.1)), yco, fieldWidth, fontHeight),results[i][j].disname,QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
                                currYco=fontHeight;
                                setPrinterFont(9);
                            }
                            if (ll%2 != 0 && j==0) drawHighlightRect(yco+currYco);
                            painter.drawText(QRectF((pr.x()+mmToPixel(10.6)+j*fieldWidth+j*mmToPixel(1.1)), yco+currYco, fieldWidth,fontHeight),results[i][j].result,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                            nextyco = qMax(nextyco,(int)(yco+currYco+fontHeight));
                            if (results[i][j].detail != "") {
                                if (ll%2 != 0 && j==0) drawHighlightRect(yco+currYco+fontHeight);
                                painter.drawText(QRectF((pr.x()+mmToPixel(10.6)+j*fieldWidth+j*mmToPixel(1.1)), yco+currYco+fontHeight, fieldWidth, fontHeight),"("+results[i][j].detail+")",QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                                nextyco = qMax(nextyco,(int)(yco+currYco+fontHeight+fontHeight));
                            }
                        }
                    }
                    yco = nextyco;
                }
            }
            yco += mmToPixel(1.0);
        }
        yco += mmToPixel(5.0);

    }
    finishPrint();
}

void Detail::printSubHeader() {
    setPrinterFont(10);
    QString jg;
    int fieldWidth;
    if (printAW) {
        fieldWidth = mmToPixel(26.0);
    } else {
        fieldWidth = mmToPixel(22.0);
    }
    if (_global::checkTyp(currWK)==0) jg = "Jg.";
    if (_global::checkTyp(currWK)==0 || _global::checkTyp(currWK)==2) {
        drawStandardRow("Platz","Name",jg,"Verein","Punkte",readDetailInfo(true));
    } else {
        drawStandardRow("Platz","Verein","","Mannschaft","Punkte",readDetailInfo(true));
    }
    if (!currWahl) {
        QSqlQuery columns;
        if (_global::checkTyp(currWK) == 1) {
            columns.prepare("SELECT var_kurz1, var_icon, CASE WHEN tfx_wettkaempfe_x_disziplinen.bol_kp='true' OR tfx_wettkaempfe.bol_kp='true' THEN 1 ELSE 0 END, bol_mansort, int_wettkaempfe_x_disziplinenid FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung DESC");
        } else {
            columns.prepare("SELECT var_kurz1, var_icon, CASE WHEN tfx_wettkaempfe_x_disziplinen.bol_kp='true' OR tfx_wettkaempfe.bol_kp='true' THEN 1 ELSE 0 END, bol_mansort, int_wettkaempfe_x_disziplinenid FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_nummer=? ORDER BY int_sortierung");
        }
        columns.bindValue(0,_global::checkHWK());
        columns.bindValue(1,currWK);
        columns.exec();
        double x;
        int y, newyco=0;
        int column=0;
        while (columns.next()) {
            for (int i=0;i<=columns.value(2).toInt();i++) {
                int currcol = column;
                int currrow = 0;
                if (columns.value(3).toBool()) {
                    QSqlQuery posquery;
                    posquery.prepare("SELECT int_sortx, int_sorty FROM tfx_wettkaempfe_dispos WHERE int_wettkaempfe_x_disziplinenid=? AND int_kp=?");
                    posquery.bindValue(0,columns.value(4).toInt());
                    posquery.bindValue(1,i);
                    posquery.exec();
                    posquery.next();
                    currcol = posquery.value(0).toInt();
                    currrow = posquery.value(1).toInt();
                }
                if (_global::checkTyp(currWK)==1) {
                    x = pr.width()-pr.x()-mmToPixel(13.0)-(currcol+1)*fieldWidth-(currcol+1)*mmToPixel(1.1);
                } else {
                    x = pr.x()+mmToPixel(10.6)+currcol*fieldWidth+currcol*mmToPixel(1.1);
                }
                y = yco+currrow*mmToPixel(5.0);
                newyco = qMax(newyco,y);
                if(Settings::iconMode == 1) {
                    QString head(columns.value(0).toString());
                    if (columns.value(2).toInt() == 1) {
                        if (i==0) {
                            head += " (P)";
                        } else {
                            head += " (K)";
                        }
                    }
                    setPrinterFont(7);
                    painter.drawText(QRectF(x, y, fieldWidth, mmToPixel(4.0)),head,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                } else {
                    QImage pm;
                    pm.load(columns.value(1).toString());
                    if (!pm.isNull()) {
                        QImage pm2 = pm.scaled(mmToPixel(4.0),mmToPixel(4.0),Qt::KeepAspectRatio,Qt::SmoothTransformation);
                        painter.drawImage(x+((fieldWidth-pm2.width())/2),y,pm2);
                        if (columns.value(2).toInt() == 1) {setPrinterFont(7);
                            QString head;
                            if (i==0) {
                                head += " (P)";
                            } else {
                                head += " (K)";
                            }
                            painter.drawText(QRectF(x+((fieldWidth-pm2.width())/2)+pm2.width(), y, fieldWidth, mmToPixel(4.0)),head,QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
                        }
                    }
                }
                column++;
            }
        }
        yco = newyco + mmToPixel(4.0);
    }
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(0.5);
}
