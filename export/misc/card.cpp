#include "card.h"
#include "model/entity/competition.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"

void Card::print(QPrinter *printer) {
    setTypeString("Punkte");
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void Card::printContent() {
    QSqlQuery query;
    query.prepare("SELECT var_nummer, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN " + _global::nameFormat() + " ELSE tfx_gruppen.var_name END AS tnname, CASE WHEN tfx_gruppen.int_gruppenid IS NULL THEN v1.var_name ELSE v2.var_name END AS vereinname, "+_global::date("dat_geburtstag",4)+", int_startnummer, int_wertungenid, var_comment FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid)  LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_vereine AS v1 ON v1.int_vereineid = tfx_teilnehmer.int_vereineid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_vereine AS v2 ON v2.int_vereineid = tfx_gruppen.int_vereineid WHERE int_wertungenid IN ("+teilnehmerString+")");
    query.exec();
    while (query.next()) {
        if (query.at()>0) {
            newPage();
        }

        Competition *competition = Competition::getByNumber(this->event, query.value(0).toString());

        setPrinterFont(12,true);
        painter.drawText(QRectF(pr.x(), yco, (pr.width()-pr.x()-pr.x())/2, QFontMetricsF(painter.font()).height()),query.value(1).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.width()/2-mmToPixel(7.5), yco, (pr.width()-pr.x()-pr.x())/2, QFontMetricsF(painter.font()).height()),"Startnummer: " + query.value(4).toString(),QTextOption(Qt::AlignVCenter));
        if (Settings::barCodes == 0) {
            QFont cbarFont = Settings::barCodeFont;
            cbarFont.setPixelSize(mmToPixel(8.0));
            painter.setFont(cbarFont);
            painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(35.0), yco, mmToPixel(35.0), QFontMetricsF(painter.font()).height()),"*" + query.value(4).toString() + "*",QTextOption(Qt::AlignRight));
            painter.setFont(font);
        }
        yco += mmToPixel(6.6);
        setPrinterFont(11);
        painter.drawText(QRectF(pr.x(), yco, (pr.width()-pr.x()-pr.x())/2, QFontMetricsF(painter.font()).height()),query.value(2).toString(),QTextOption(Qt::AlignVCenter));
        if (competition->getType() != 2) painter.drawText(QRectF(pr.width()/2-mmToPixel(7.5), yco, (pr.width()-pr.x()-pr.x())/2, QFontMetricsF(painter.font()).height()),"Jahrgang: " + query.value(3).toString(),QTextOption(Qt::AlignVCenter));
        yco += mmToPixel(6.6);
        printDescriptor(query.value(0).toString());
        int yco2 = yco;
        QSqlQuery query9;
        query9.prepare("SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen INNER JOIN tfx_wertungen USING (int_wertungenid) INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wertungen.int_wettkaempfeid WHERE int_wertungenid=?");
        query9.bindValue(0,query.value(5).toInt());
        query9.exec();
        QSqlQuery query2;
        if (_global::querySize(query9) == 0) {
            query2.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_kurz2, int_versuche, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_wertungenid=? ORDER BY int_sortierung");
        } else {
            query2.prepare("SELECT int_disziplinenid, tfx_disziplinen.var_kurz2, int_versuche, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_wettkaempfeid = tfx_wettkaempfe_x_disziplinen.int_wettkaempfeid INNER JOIN tfx_wertungen ON tfx_wertungen.int_wettkaempfeid = tfx_wettkaempfe.int_wettkaempfeid WHERE int_wertungenid=? AND int_disziplinenid IN (SELECT int_disziplinenid FROM tfx_wertungen_x_disziplinen WHERE int_wertungenid=tfx_wertungen.int_wertungenid) ORDER BY int_sortierung");
        }
        query2.bindValue(0,query.value(5).toInt());
        query2.exec();
        bool col2 = false;
        while (query2.next()) {
            for (int kp=0;kp<=query2.value(3).toInt();kp++) {
                int xco;
                if (!col2) {
                    xco = pr.x();
                } else {
                    xco = pr.width()/2;
                }
                setPrinterFont(10,true);
                QString name = query2.value(1).toString();
                QSqlQuery checkKuer;
                checkKuer.prepare("SELECT int_wettkaempfeid FROM tfx_wettkaempfe INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) INNER JOIN tfx_veranstaltungen USING (int_veranstaltungenid) WHERE int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=? AND (tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true')");
                checkKuer.bindValue(0, this->event->mainEventId());
                checkKuer.bindValue(1,query.value(0).toString());
                checkKuer.exec();
                if (_global::querySize(checkKuer)>0) {
                    if (kp == 0) {
                        name += " (P)";
                    } else {
                        name += " (K)";
                    }
                }
                painter.drawText(QRectF(xco, yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),name,QTextOption(Qt::AlignVCenter));
                yco += mmToPixel(4.0);
                setPrinterFont(8,true);
                int vwidth=0;
                bool nextRow=false;
                for (int i=0;i<query2.value(2).toInt();i++) {
                    QSqlQuery fields;
                    fields.prepare("SELECT var_name FROM tfx_disziplinen_felder WHERE int_disziplinenid=? AND bol_enabled='true' ORDER BY int_sortierung");
                    fields.bindValue(0,query2.value(0).toInt());
                    fields.exec();
                    while (fields.next()) {
                        if (!nextRow) painter.drawText(QRectF(xco, yco, mmToPixel(15.9), QFontMetricsF(painter.font()).height()),fields.value(0).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
                        if (nextRow && fields.at()==0) yco-= mmToPixel(4.0);
                        painter.drawRect(QRectF(xco, yco+mmToPixel(4.0), mmToPixel(15.9), mmToPixel(6.6)));
                        xco = xco+mmToPixel(15.9);
                    }
                    xco = xco + mmToPixel(2.6);
                    vwidth = _global::querySize(fields)*mmToPixel(18.5);
                    if (vwidth > pr.width()-pr.x()-xco && i<query2.value(2).toInt()-1) {
                        if (!col2) {
                            xco = pr.x();
                        } else {
                            xco = pr.width()/2;
                        }
                        yco += mmToPixel(11.9);
                        nextRow=true;
                    }
                }
                if (yco + mmToPixel(11.9) > pr.height()-pr.y()-mmToPixel(11.9)) {
                    yco = yco2-mmToPixel(11.9);
                    col2 = true;
                }
                yco += mmToPixel(11.9);
            }
        }
        int rows = 0;
        if (competition->getType() == 2) {
            setPrinterFont(8);
            QSqlQuery teamq2;
            QString teamstr;
            teamstr = "SELECT " + _global::nameFormat() + " || ' (' || ";
            teamstr += _global::date("dat_geburtstag",2);
            teamstr += " || ')' FROM tfx_gruppen_x_teilnehmer INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_wertungen ON tfx_wertungen.int_gruppenid = tfx_gruppen_x_teilnehmer.int_gruppenid WHERE int_wertungenid=? ORDER BY var_nachname, var_vorname";
            teamq2.prepare(teamstr);
            teamq2.bindValue(0,query.value(5).toInt());
            teamq2.exec();
            int column=0;
            rows = _global::querySize(teamq2)/3+1;
            yco = pr.height()-pr.x()-mmToPixel(5.0)-rows*mmToPixel(3.4);
            while (teamq2.next()) {
                painter.drawText(QRectF(pr.x()+column*((pr.width()-pr.x()-mmToPixel(10.6))/3), yco, ((pr.width()-pr.x()-mmToPixel(10.6))/3), QFontMetricsF(painter.font()).height()),teamq2.value(0).toString(),QTextOption(Qt::AlignVCenter));
                column++;
                if (column == 3) {
                    column = 0;
                    yco += mmToPixel(3.2);
                }
            }
        }
        painter.drawRect(QRectF(pr.width()-pr.x()-mmToPixel(20.0),pr.height()-pr.y()-mmToPixel(20.0)-rows*mmToPixel(3.5),mmToPixel(20.0),mmToPixel(8.0)));
        painter.drawRect(QRectF(pr.width()-pr.x()-mmToPixel(20.0),pr.height()-pr.y()-mmToPixel(28.0)-rows*mmToPixel(3.5),mmToPixel(20.0),mmToPixel(8.0)));
        painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(60.0),pr.height()-pr.y()-mmToPixel(20.0)-rows*mmToPixel(3.5), mmToPixel(38.0), mmToPixel(8.0)),"Platz:",QTextOption(Qt::AlignVCenter | Qt::AlignRight));
        painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(60.0),pr.height()-pr.y()-mmToPixel(28.0)-rows*mmToPixel(3.5), mmToPixel(38.0), mmToPixel(8.0)),"Gesamtpunktzahl:",QTextOption(Qt::AlignVCenter | Qt::AlignRight));
        painter.drawText(QRectF(pr.width()-pr.x()-mmToPixel(100.0),pr.height()-pr.y()-mmToPixel(36.0)-rows*mmToPixel(3.5), mmToPixel(100.0), mmToPixel(8.0)),query.value(6).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignRight));
    }
    finishPrint();
}
