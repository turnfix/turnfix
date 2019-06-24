#include "resultssheet.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"

void ResultsSheet::printContent() {
    for (int i=0;i<riegenNumbers.size();i++) {
        currRiege = riegenNumbers.at(i);

        QSqlQuery query2;
        query2.prepare("SELECT tfx_wertungen.int_startnummer, CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN tfx_gruppen.var_name ELSE " + _global::nameFormat() + " || CASE WHEN tfx_wertungen.bol_ak='true' THEN ' (AK)' ELSE '' END END, CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN v1.var_name || ' - ' || tfx_mannschaften.int_nummer || '. M.' ELSE CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN v2.var_name ELSE v3.var_name END END, tfx_wettkaempfe.var_nummer, "+_global::date("dat_geburtstag",2)+", int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_vereine AS v3 ON tfx_teilnehmer.int_vereineid = v3.int_vereineid LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_vereine AS v1 ON tfx_mannschaften.int_vereineid = v1.int_vereineid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_vereine AS v2 ON tfx_gruppen.int_vereineid = v2.int_vereineid WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND tfx_wertungen.var_riege=? AND tfx_wertungen.int_runde=? AND tfx_wettkaempfe_x_disziplinen.int_disziplinenid=? AND (NOT EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) OR EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE tfx_wertungen_x_disziplinen.int_wertungenid=tfx_wertungen.int_wertungenid AND tfx_wertungen_x_disziplinen.int_disziplinenid=?)) AND tfx_wertungen.bol_startet_nicht='false' ORDER BY tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer, tfx_mannschaften.int_mannschaftenid, tfx_wertungen.int_startnummer");
        query2.bindValue(0, this->event->getMainEventId());
        query2.bindValue(1, currRiege);
        query2.bindValue(2, this->event->getRound());
        for (int j=0;j<disziplinenIDs.size();j++) {

            currDis = disziplinenIDs.at(j).at(0);

            QSqlQuery disinfo;
            disinfo.prepare("SELECT var_name, var_icon, int_versuche FROM tfx_disziplinen WHERE int_disziplinenid=? LIMIT 1");
            disinfo.bindValue(0,currDis);
            disinfo.exec();
            disinfo.next();


            query2.bindValue(3,currDis);
            query2.bindValue(4,currDis);
            query2.exec();
            if (!(i==0 && j==0)) {
                newPage();
            }

            QString name = disinfo.value(0).toString();
            QSqlQuery checkKuer;
            checkKuer.prepare("SELECT int_wettkaempfeid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
            checkKuer.bindValue(0, this->event->getMainEventId());
            checkKuer.bindValue(1, disziplinenIDs.at(j).at(0));
            checkKuer.exec();
            if (_global::querySize(checkKuer)>0) {
                if (disziplinenIDs.at(j).at(1) == 0) {
                    name += " (P)";
                } else {
                    name += " (K)";
                }
            }

            printType(disinfo.value(1).toString(),name);

            setPrinterFont(10);
            QString lastWK="";
            int rows=0;
            QString lastTeam="";
            while (query2.next()) {
                QSqlQuery fields;
                fields.prepare("SELECT var_name, rel_leistung FROM tfx_disziplinen_felder LEFT JOIN tfx_jury_results ON tfx_jury_results.int_disziplinen_felderid = tfx_disziplinen_felder.int_disziplinen_felderid AND int_wertungenid=? WHERE int_disziplinenid=? AND bol_endwert='false' AND bol_enabled='true' ORDER BY int_sortierung");
                fields.bindValue(1,disziplinenIDs.at(j).at(0));
                fields.bindValue(0,query2.value(5).toInt());
                fields.exec();
                double max=0;
                while (fields.next()) {
                    max = qMax(max,fields.value(1).toDouble());
                }
                if (max==0) continue;
                currWK = query2.value(3).toString();
                checkKuer.prepare("SELECT int_wettkaempfeid FROM tfx_wettkaempfe INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
                checkKuer.bindValue(0, this->event->getMainEventId());
                checkKuer.bindValue(1, currWK);
                checkKuer.exec();
                if (_global::querySize(checkKuer)==0 && name.right(3) == "(K)") {
                    continue;
                }
                double skip = 11.9;
                if (rows>0) {
                    skip = skip + rows * 7.2;
                }
                    if (checkWKChange(currWK,lastWK,skip)) {
                        printType(disinfo.value(1).toString(),name);
                    }
                rows=0;
                setPrinterFont(10);
                lastTeam = query2.value(2).toString();
                drawStandardRow(query2.value(0).toString() + "  ",query2.value(1).toString(),query2.value(4).toString(),query2.value(2).toString(),"",readDetailInfo(false,lastTeam));
                int xco = pr.x()+mmToPixel(10.6);
                int fcount=0;
                for (int k=0;k<disinfo.value(2).toInt();k++) {
                    if (k>0) {
                        if ((fcount*mmToPixel(15.9))>pr.width()-pr.x()-pr.x()-xco-mmToPixel(2.6)) {
                            xco= pr.x()+mmToPixel(10.6);
                            yco += mmToPixel(7.2);
                            rows++;
                        } else {
                            xco=xco+mmToPixel(2.6);
                        }
                    }
                    fields.exec();
                    fcount = _global::querySize(fields);
                    while (fields.next()) {
                        painter.drawRect(QRectF(xco, yco, mmToPixel(15.9), mmToPixel(6.6)));
                        painter.drawText(QRectF(xco, yco, mmToPixel(15.9), mmToPixel(6.6)),fields.value(1).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
                        xco += mmToPixel(15.9);
                    }
                }
                yco += mmToPixel(7.4);
                painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
                yco += mmToPixel(0.3);
                lastWK = query2.value(3).toString();
            }
        }

    }
    finishPrint();
}

void ResultsSheet::printSubHeader() {
    List::printSubHeader();
    setPrinterFont(7);
    int xco = pr.x()+mmToPixel(10.6);
    QSqlQuery fields;
    fields.prepare("SELECT var_name FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND bol_endwert='false' AND bol_enabled='true' ORDER BY int_sortierung");
    fields.bindValue(0,currDis);
    fields.exec();
    while (fields.next()) {
        painter.drawText(QRectF(xco, yco, mmToPixel(15.9), QFontMetricsF(painter.font()).height()),fields.value(0).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
        xco = xco+mmToPixel(15.9);
    }
    yco += mmToPixel(3.5);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += 1;
}

void ResultsSheet::printType(QString file,QString name) {
    int off=0;
    QString geraet;
    if(Settings::iconMode == 0) {
        off = mmToPixel(12.0);
        QImage pm;
        pm.load(file);
        QImage pm2 = pm.scaled(mmToPixel(10.6),mmToPixel(10.6),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        painter.drawImage(pr.width()-pr.x()-pm2.width(),pr.y()+mmToPixel(1.3),pm2);
    } else {
        geraet = name;
    }
    setPrinterFont(30,true);
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)-off), (pr.y()-mmToPixel(0.5)), mmToPixel(105), QFontMetricsF(painter.font()).height()),"Riege "+currRiege,QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    if(Settings::iconMode == 1) {
        painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)), (pr.y()-mmToPixel(1.5)+fontHeight), mmToPixel(105), QFontMetricsF(painter.font()).height()),geraet,QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    } else {
        if (name.right(3)=="(K)" || name.right(3)=="(P)") {
            painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)), (pr.y()-mmToPixel(1.5)+fontHeight), mmToPixel(105), QFontMetricsF(painter.font()).height()),name.right(3),QTextOption(Qt::AlignRight | Qt::AlignVCenter));
        }
    }
    if (Settings::barCodes == 0) {
        QFont cbarFont(Settings::barCodeFont);
        cbarFont.setPixelSize(20);
        painter.setFont(cbarFont);
        painter.drawText(QRectF(pr.x(), (pr.height()-pr.y()-mmToPixel(3.2)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"*" + currRiege + "-" + QString().setNum(currDis) + "*",QTextOption(Qt::AlignCenter));
        painter.setFont(font);
    }
}
