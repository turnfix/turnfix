#include "judgessheet.h"
#include "model/entity/event.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"

bool JudgesSheet::teammode = false;

void JudgesSheet::setTeammode(bool set) {
    teammode = set;
}

void JudgesSheet::printContent() {
    for (int i=0;i<riegenNumbers.size();i++) {
        currRiege = riegenNumbers.at(i);

        QSqlQuery erstesGeraet;
        erstesGeraet.prepare("SELECT int_disziplinenid FROM tfx_riegen_x_disziplinen WHERE int_veranstaltungenid=? AND int_runde=? AND var_riege=? AND bol_erstes_geraet='true'");
        erstesGeraet.bindValue(0, this->m_event->mainEvent()->id());
        erstesGeraet.bindValue(1, this->m_event->round());
        erstesGeraet.bindValue(2, currRiege);
        erstesGeraet.exec();

        int position=0;

        if (_global::querySize(erstesGeraet)>0) {
            erstesGeraet.next();
            int erstesGeraetID = erstesGeraet.value(0).toInt();
            for (int j=0;j<disziplinenIDs.size();j++) {
                if (disziplinenIDs.at(j).at(0) == erstesGeraetID) {
                    position = j;
                    break;
                }
            }
        }

        QSqlQuery query2;
        query2.prepare("SELECT tfx_wertungen.int_startnummer, CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN tfx_gruppen.var_name ELSE " + _global::nameFormat() + " || CASE WHEN tfx_wertungen.bol_ak='true' THEN ' (AK)' ELSE '' END END, CASE WHEN tfx_wertungen.int_mannschaftenid IS NOT NULL THEN v1.var_name || ' - ' || tfx_mannschaften.int_nummer || '. M.' ELSE CASE WHEN tfx_wertungen.int_gruppenid IS NOT NULL THEN v2.var_name ELSE v3.var_name END END, tfx_wettkaempfe.var_nummer, "+_global::date("dat_geburtstag",2)+", s1.int_pos FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_vereine AS v3 ON tfx_teilnehmer.int_vereineid = v3.int_vereineid LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_vereine AS v1 ON tfx_mannschaften.int_vereineid = v1.int_vereineid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_vereine AS v2 ON tfx_gruppen.int_vereineid = v2.int_vereineid LEFT JOIN tfx_startreihenfolge AS s1 ON s1.int_wertungenid=tfx_wertungen.int_wertungenid AND s1.int_disziplinenid=? AND s1.int_kp=? WHERE tfx_wettkaempfe.int_veranstaltungenid=? AND tfx_wertungen.var_riege=? AND tfx_wertungen.int_runde=? AND tfx_wettkaempfe_x_disziplinen.int_disziplinenid=? AND (NOT EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) OR EXISTS (SELECT int_wertungen_x_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE tfx_wertungen_x_disziplinen.int_wertungenid=tfx_wertungen.int_wertungenid AND tfx_wertungen_x_disziplinen.int_disziplinenid=?)) AND tfx_wertungen.bol_startet_nicht='false' ORDER BY int_pos, tfx_wettkaempfe.var_nummer, tfx_mannschaften.int_nummer, tfx_mannschaften.int_mannschaftenid, tfx_wertungen.int_startnummer");

        query2.bindValue(1,0);
        query2.bindValue(2, this->m_event->mainEvent()->id());
        query2.bindValue(3, currRiege);
        query2.bindValue(4, this->m_event->round());
        for (int j=0;j<disziplinenIDs.size();j++) {

            currDis = disziplinenIDs.at(position).at(0);

            if (position == disziplinenIDs.size()-1) {
                position = 0;
            } else {
                position++;
            }

            QSqlQuery disinfo;
            disinfo.prepare("SELECT var_name, var_icon, int_versuche FROM tfx_disziplinen WHERE int_disziplinenid=? LIMIT 1");
            disinfo.bindValue(0,currDis);
            disinfo.exec();
            disinfo.next();

            if (!(i==0 && j==0)) {
                newPage();
            }

            QString name = disinfo.value(0).toString();
            QSqlQuery checkKuer;
            checkKuer.prepare("SELECT int_wettkaempfeid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE int_veranstaltungenid=? AND int_disziplinenid=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
            checkKuer.bindValue(0, this->m_event->mainEvent()->id());
            checkKuer.bindValue(1,disziplinenIDs.at(j).at(0));
            checkKuer.exec();
            if (_global::querySize(checkKuer)>0) {
                if (disziplinenIDs.at(j).at(1) == 0) {
                    name += " (P)";
                } else {
                    name += " (K)";
                }
                query2.bindValue(1,disziplinenIDs.at(j).at(1));
            }

            query2.bindValue(0,currDis);
            query2.bindValue(5,currDis);
            query2.bindValue(6,currDis);
            query2.exec();

            bool skipSR=false;
            if (_global::querySize(query2)>0) {
                query2.last();
                if (query2.value(5).toInt()>0) skipSR = true;
                query2.first();
                if (query2.value(5).toInt()>0) skipSR = true;
                query2.seek(-1);
            }

            printType(disinfo.value(1).toString(),name);

            setPrinterFont(10);
            QString lastWK="";
            int rows=0;
            QString lastTeam="";
            while (query2.next()) {
                if (query2.value(5).toInt()==0 && skipSR) continue;
                currWK = query2.value(3).toString();
                checkKuer.prepare("SELECT int_wettkaempfeid FROM tfx_wettkaempfe INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
                checkKuer.bindValue(0, this->m_event->mainEvent()->id());
                checkKuer.bindValue(1,currWK);
                checkKuer.exec();
                if (_global::querySize(checkKuer)==0 && name.right(3) == "(K)") {
                    continue;
                }
                double skip = 11.9;
                if (rows>0) {
                    skip = skip + rows * 7.2;
                }
                if (teammode && query2.value(2).toString() != lastTeam && lastTeam != "") {
                    newPage();
                    printDescriptor(currWK);
                    printType(disinfo.value(1).toString(),name);
                } else {
                    if (checkWKChange(currWK,lastWK,skip)) {
                        printType(disinfo.value(1).toString(),name);
                    }
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
                    QSqlQuery fields;
                    fields.prepare("SELECT var_name FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND bol_enabled='true' ORDER BY int_sortierung");
                    fields.bindValue(0,disziplinenIDs.at(j).at(0));
                    fields.exec();
                    fcount = _global::querySize(fields);
                    while (fields.next()) {
                        painter.drawRect(QRectF(xco, yco, mmToPixel(15.9), mmToPixel(6.6)));
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

void JudgesSheet::printSubHeader() {
    List::printSubHeader();
    setPrinterFont(7);
    int xco = pr.x()+mmToPixel(10.6);
    QSqlQuery fields;
    fields.prepare("SELECT var_name FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND bol_enabled='true' ORDER BY int_sortierung");
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

void JudgesSheet::printType(QString file,QString name) {
    int off=0;
    QString geraet = name;
    if(Settings::iconMode == 0) {
        off = mmToPixel(12.0);
        QImage pm;
        pm.load(file);
        QImage pm2 = pm.scaled(mmToPixel(10.6),mmToPixel(10.6),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        painter.drawImage(pr.width()-pr.x()-pm2.width(),pr.y()+mmToPixel(1.3),pm2);
    }
    setPrinterFont(30,true);
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)-off), (pr.y()-mmToPixel(0.5)), mmToPixel(105), QFontMetricsF(painter.font()).height()),"Riege "+currRiege,QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    if(Settings::iconMode == 1) {
        painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)), (pr.y()-mmToPixel(1.5)+fontHeight), mmToPixel(105), QFontMetricsF(painter.font()).height()),geraet,QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    } else {
        setPrinterFont(11,true);
        painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(150.0)), (pr.y()+mmToPixel(13.0)), mmToPixel(150.0), QFontMetricsF(painter.font()).height()),name,QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    }
    if (Settings::barCodes == 0) {
        QFont cbarFont = Settings::barCodeFont;
        cbarFont.setPixelSize(mmToPixel(8.0));
        painter.setFont(cbarFont);
        painter.drawText(QRectF(pr.x(), (pr.height()-pr.y()-mmToPixel(3.2)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"*" + currRiege + "-" + QString().setNum(currDis) + "*",QTextOption(Qt::AlignCenter));
        painter.setFont(font);
    }
}
