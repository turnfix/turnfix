#include "print.h"
#include "misc/certificate.h"
#include "model/entity/competition.h"
#include "src/global/header/_global.h"
#include "src/global/header/settings.h"
#include <QDateTime>
#include <QSqlQuery>

int Print::detailinfo = 0;
int Print::coverID = 0;

QPrinter::PaperSize Print::paperSize = QPrinter::A4;
QPrinter::Orientation Print::orientation = QPrinter::Portrait;

int Print::headFootID = 0;

Print::Print(Event *event) : QThread() {
    this->event = event;
    showPreview = false;
    selectClub = false;
    selectTN = false;
    selectWK = false;
    selectRiege = false;
    selectDis = false;
    selectDetail = false;
    typeString = "";
    outputFileName = "";
    singleWK = "";
    finish = false;
}

void Print::setOutputType(int set) {
    outputType = set;
}

int Print::getOutputType() {
    return outputType;
}

void Print::setDetailInfo(int set) {
    detailinfo = set;
}

void Print::setShowPreview(bool set) {
    showPreview = set;
}

void Print::setSelectClub(bool set) {
    selectClub = set;
}

void Print::setSelectRiege(bool set) {
    selectRiege = set;
}

void Print::setSelectTN(bool set) {
    selectTN = set;
}

void Print::setSelectWK(bool set) {
    selectWK = set;
}

void Print::setSelectDis(bool set) {
    selectDis = set;
}

void Print::setSelectDetail(bool set) {
    selectDetail = set;
}

void Print::setPaperSize(QPrinter::PaperSize set) {
    paperSize = set;
}

QPrinter::PaperSize Print::getPaperSize() {
    return paperSize;
}

void Print::setOrientation(QPrinter::Orientation set) {
    orientation = set;
}

QPrinter::Orientation Print::getOrientation() {
    return orientation;
}

void Print::setHeadFootID(int set) {
    headFootID = set;
}

void Print::setCoverID(int set) {
    coverID = set;
}

void Print::setOutputFileName(QString set) {
    outputFileName = set;
}

void Print::setWKNumber(QString set) {
    singleWK = set;
}

void Print::setVerein(int set) {
    QList<int> lst;
    lst.append(set);
    vereinNumbers = lst;
}

void Print::setDetailQuery(QString set) {
    detailQuery = set;
}

void Print::setSelectedClubs(QList<int> set) {
    selectedClubs = set;
}

void Print::setSelectedDisziplinen(QList<QList<int> > set) {
    selectedDisziplinen = set;
}

void Print::setSelectedRiegen(QStringList set) {
    selectedRiegen = set;
}

void Print::setSelectedTN(QList<int> set) {
    selectedTN = set;
}

void Print::setSelectedTNWK(QString set) {
    selectedTNWK = set;
}

void Print::setSelectedWKs(QStringList set) {
    selectedWKs = set;
}

void Print::setTeilnehmerNumbers(QList<int> set) {
    teilnehmerNumbers = set;
}

void Print::setFinish(bool set) {
    finish = set;
}

void Print::printHeadFoot() {

    if (headFootID>0) {
        printCustomPage(2,headFootID);
    } else {
        painter.drawLine(QPointF(pr.x(),pr.y()),QPointF(pr.width()-pr.x(),pr.y()));
        setPrinterFont(10);
        QSqlQuery query;
        query.prepare("SELECT var_veranstalter, tfx_veranstaltungen.var_name, "+_global::date("dat_von",10)+", tfx_wettkampforte.var_name, "+_global::date("dat_bis",10)+", var_ort FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=? LIMIT 1");
        query.bindValue(0, this->event->id());
        query.exec();
        query.next();
        QString datum;
        if (query.value(2).toString() == query.value(4).toString()) {
            datum = query.value(2).toString();
        } else {
            datum = query.value(2).toString() + " - " + query.value(4).toString();
        }
        painter.drawText(QRectF(pr.x(), (pr.y()+mmToPixel(1.3)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),query.value(0).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x(), (pr.y()+mmToPixel(4.8)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),query.value(1).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x(), (pr.y()+mmToPixel(8.2)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),datum + " " + query.value(3).toString() + " " + query.value(5).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawLine(QPointF(pr.x(),pr.y()+mmToPixel(13.0)),QPointF(pr.width()-pr.x(),pr.y()+mmToPixel(13.0)));

        // Type
        setPrinterFont(30,true);
        painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(105.9)/*-off*/), (pr.y()-mmToPixel(0.5)), mmToPixel(105), QFontMetricsF(painter.font()).height()),typeString,QTextOption(Qt::AlignRight | Qt::AlignVCenter));


        yco = pr.y()+mmToPixel(15.0);


        painter.drawLine(QPointF(pr.x(),pr.height()-pr.y()-mmToPixel(4.5)),QPointF(pr.width()-pr.x(),pr.height()-pr.y()-mmToPixel(4.5)));

        font.setFamily("Tahoma");
        painter.setFont(font);
    }
    setPrinterFont(8);
    QString extra;
    if(Settings::creator.length() > 0) {
        extra = " durch " + Settings::creator;
    }
    painter.drawText(QRectF(pr.x(), (pr.height()-pr.y()-mmToPixel(3.2)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"Erstellt" + extra + " mit TurnFix - www.turnfix.de",QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x(), (pr.height()-pr.y()-mmToPixel(3.2)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),QDateTime::currentDateTime().toString("dd.MM.yyyy - HH:mm") + " Uhr",QTextOption(Qt::AlignVCenter | Qt::AlignRight));
    setPrinterFont(6);
    painter.drawText(QRectF(pr.x(), (pr.height()-pr.y()-mmToPixel(0.5)), pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"Lizenziert für: " + Settings::organisation,QTextOption(Qt::AlignVCenter | Qt::AlignRight));
}

void Print::printDescriptor(QString swknr, bool f) {
    QSqlQuery query;
    query.prepare("SELECT var_nummer, var_name FROM tfx_wettkaempfe WHERE var_nummer=? AND int_veranstaltungenid=?");
    query.bindValue(0,swknr);
    query.bindValue(1, this->event->mainEventId());
    query.exec();
    query.next();
    QString fort;
    if (f > 0) {
        fort = QString(" (Fortsetzung)");
    } else {
        fort = QString("");
    }
    setPrinterFont(14,true);
    painter.drawText(QRectF(pr.x(), yco-mmToPixel(2.6), pr.width()-pr.x()-pr.x(), (QFontMetricsF(painter.font()).height())*2.3),"WK Nr. " + query.value(0).toString() + " " + query.value(1).toString() + _global::wkBez(this->event, swknr) + fort,QTextOption(Qt::AlignVCenter));
    yco += QFontMetricsF(painter.font()).height()*2.3;
    printSubHeader();
}

void Print::printContent() {}

void Print::printSubHeader() {}

void Print::newPage(bool print) {
    curr_printer->newPage();
    yco = top_yco;
    if (print) printHeadFoot();
}

void Print::finishPrint() {
    painter.end();
    customImages.clear();
}

void Print::run() {

    //Detailselektion
    detailQuery="";
    if (selectDetail && detailinfo > 0) {
        emit requestDetailInfo();
    }

    //Vereinsselektion
    if (outputType != 2) {
        QSqlQuery vereineQuery;
        vereineQuery.prepare("SELECT tfx_vereine.int_vereineid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) LEFT JOIN tfx_teilnehmer ON tfx_teilnehmer.int_teilnehmerid = tfx_wertungen.int_teilnehmerid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid INNER JOIN tfx_vereine ON tfx_vereine.int_vereineid = tfx_teilnehmer.int_vereineid OR tfx_vereine.int_vereineid = tfx_gruppen.int_vereineid OR tfx_vereine.int_vereineid = tfx_mannschaften.int_vereineid WHERE int_veranstaltungenid=? GROUP BY tfx_vereine.int_vereineid, tfx_vereine.var_name, tfx_vereine.int_start_ort, tfx_gruppen.int_gruppenid ORDER BY  tfx_vereine.var_name");
        vereineQuery.bindValue(0, this->event->mainEventId());
        vereineQuery.exec();
        while (vereineQuery.next()) {
            vereinNumbers.append(vereineQuery.value(0).toInt());
        }
        if (selectClub) {
            emit requestVereine();
            if (finish) return;

            for (int i=vereinNumbers.size()-1;i>=0;i--) {
                if (!selectedClubs.contains(vereinNumbers.at(i))) {
                    vereinNumbers.removeAt(i);
                }
            }
        }
    }

    //Wettkampfselektion
    QString boolor;
    if (_global::getDBTyp()==0) {
        boolor = "bool_or";
    } else {
        boolor = "max";
    }
    QSqlQuery wkQuery;
    if (!selectClub) {
        wkQuery.prepare("SELECT var_nummer, bol_wahlwettkampf, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR "+boolor+"(tfx_wettkaempfe_x_disziplinen.bol_kp)='true' THEN 'true' ELSE 'false' END FROM tfx_wettkaempfe INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND (SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid) > 0 GROUP BY var_nummer, bol_wahlwettkampf, tfx_wettkaempfe.bol_kp ORDER BY var_nummer");
    } else {
        QString query = "SELECT var_nummer, bol_wahlwettkampf, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR "+boolor+"(tfx_wettkaempfe_x_disziplinen.bol_kp)='true' THEN 'true' ELSE 'false' END FROM tfx_wettkaempfe INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND (SELECT COUNT(*) FROM tfx_wertungen WHERE int_wettkaempfeid=tfx_wettkaempfe.int_wettkaempfeid AND int_wertungenid IN (SELECT int_wertungenid FROM tfx_wertungen LEFT JOIN tfx_teilnehmer USING (int_teilnehmerid) LEFT JOIN tfx_mannschaften ON tfx_mannschaften.int_mannschaftenid = tfx_wertungen.int_mannschaftenid LEFT JOIN tfx_gruppen ON tfx_gruppen.int_gruppenid = tfx_wertungen.int_gruppenid INNER JOIN tfx_vereine AS v ON v.int_vereineid = tfx_teilnehmer.int_vereineid OR v.int_vereineid = tfx_mannschaften.int_vereineid OR v.int_vereineid = tfx_gruppen.int_vereineid WHERE v.int_vereineid IN (" + _global::intListToString(vereinNumbers) + ") AND bol_startet_nicht='false')) > 0 GROUP BY var_nummer, bol_wahlwettkampf, tfx_wettkaempfe.bol_kp ORDER BY var_nummer";
        wkQuery.prepare(query);
    }
    wkQuery.bindValue(0, this->event->mainEventId());
    wkQuery.exec();
    wkNumbers.clear();
    wkWahl.clear();
    while (wkQuery.next()) {
        wkNumbers.append(wkQuery.value(0).toString());
        wkWahl.append(wkQuery.value(1).toBool());
        wkKP.append(wkQuery.value(2).toBool());
    }
    if (selectWK) {
        emit requestWKs();
        if (finish) return;
        if (selectWK) {
            for (int i=wkNumbers.size()-1;i>=0;i--) {
                if (!selectedWKs.contains(wkNumbers.at(i))) {
                    wkNumbers.removeAt(i);
                    wkWahl.removeAt(i);
                    wkKP.removeAt(i);
                }
            }
        }
    }
    if (singleWK.length()>0) {
        for (int i=wkNumbers.size()-1;i>=0;i--) {
            if (wkNumbers.at(i) != singleWK) {
                wkNumbers.removeAt(i);
                wkWahl.removeAt(i);
                wkKP.removeAt(i);
            }
        }
    }

    //Riegen selektion
    QSqlQuery riegenQuery;
    riegenQuery.prepare("SELECT var_riege, COUNT(DISTINCT int_teilnehmerid)+COUNT(DISTINCT int_gruppenid) as \"count\" FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? AND var_riege != '' GROUP BY var_riege ORDER BY var_riege");
    riegenQuery.bindValue(0, this->event->mainEventId());
    riegenQuery.exec();
    riegenNumbers.clear();
    while (riegenQuery.next()) {
        riegenNumbers.append(riegenQuery.value(0).toString());
    }
    if (selectRiege) {
        emit requestRiegen();
        if (finish) return;
        for (int i=riegenNumbers.size()-1;i>=0;i--) {
            if (!selectedRiegen.contains(riegenNumbers.at(i))) {
                riegenNumbers.removeAt(i);
            }
        }
    }

    //Teilnehmerselektion
    if (selectTN) {
        emit requestTN();
        if (finish) return;
        Competition *competition = Competition::getByNumber(this->event, selectedTNWK);
        if (competition->getType() == 1) {
            QList<int> selectedTeamTeilnehmer;
            QSqlQuery teamTeilnehmer;
            teamTeilnehmer.prepare("SELECT int_wertungenid FROM tfx_wertungen INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_runde=? AND tfx_wertungen.int_mannschaftenid IN ("+_global::intListToString(selectedTN)+")");
            teamTeilnehmer.bindValue(0, this->event->round());
            teamTeilnehmer.exec();
            while (teamTeilnehmer.next()) {
                selectedTeamTeilnehmer.append(teamTeilnehmer.value(0).toInt());
            }
            teilnehmerString = _global::intListToString(selectedTeamTeilnehmer);
        } else {
            teilnehmerString = _global::intListToString(selectedTN);
        }
    }

    //Disziplinen selektieren
    QSqlQuery disziplinenQuery;
    disziplinenQuery.prepare("SELECT DISTINCT int_disziplinenid, CASE WHEN tfx_wettkaempfe.bol_kp='true' OR tfx_wettkaempfe_x_disziplinen.bol_kp='true' THEN 1 ELSE 0 END as kp, tfx_disziplinen.var_name FROM tfx_disziplinen INNER JOIN tfx_wettkaempfe_x_disziplinen USING (int_disziplinenid) INNER JOIN tfx_wettkaempfe USING (int_wettkaempfeid) WHERE int_veranstaltungenid=? GROUP BY int_disziplinenid, tfx_wettkaempfe.bol_kp, tfx_wettkaempfe_x_disziplinen.bol_kp, tfx_disziplinen.var_name ORDER BY tfx_disziplinen.var_name, kp");
    disziplinenQuery.bindValue(0, this->event->mainEventId());
    disziplinenQuery.exec();
    disziplinenIDs.clear();
    while (disziplinenQuery.next()) {
        QList<int> lst;
        lst.append(disziplinenQuery.value(0).toInt());
        lst.append(0);
        disziplinenIDs.append(lst);
        if (disziplinenQuery.value(1).toInt()==1) {
            lst.replace(1,1);
            disziplinenIDs.append(lst);
        }
    }
    if (selectDis) {
        emit requestDisziplinen();
        if (finish) return;
        for (int i=disziplinenIDs.size()-1;i>=0;i--) {
            if (!selectedDisziplinen.contains(disziplinenIDs.at(i))) {
                disziplinenIDs.removeAt(i);
            }
        }
    }
    //Druckvorschau
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageMargins(5.0,5.0,5.0,5.0,QPrinter::Millimeter);
    printer->setOrientation(orientation);
    if (outputType == 0) {
        printer->setOutputFormat(QPrinter::NativeFormat);
    } else {
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setResolution(300);
    }
    if (showPreview) {
        emit showPrintPreview(printer);
    } else {
        print(printer);
    }
    delete curr_printer;
}

void Print::print(QPrinter *prt) {
    curr_printer = prt;
    if (outputType == 2) curr_printer->setOutputFileName(outputFileName);
    pr = curr_printer->pageRect();
    if (headFootID>0) {
        QSqlQuery layoutData;
        layoutData.prepare("SELECT rel_y, rel_h FROM tfx_layout_felder WHERE int_layoutid=?");
        layoutData.bindValue(0,headFootID);
        layoutData.exec();
        top_yco = 0;
        max_yco = pr.height()-pr.y();
        while (layoutData.next()) {
            if ((mmToPixel(layoutData.value(0).toDouble()) + mmToPixel(layoutData.value(1).toDouble())) < (pr.height()/2)) {
                if ((mmToPixel(layoutData.value(0).toDouble()) + mmToPixel(layoutData.value(1).toDouble()))>top_yco) {
                    top_yco = (mmToPixel(layoutData.value(0).toDouble()) + mmToPixel(layoutData.value(1).toDouble()))+mmToPixel(5.0);
                }
            } else {
                if (mmToPixel(layoutData.value(0).toDouble())<max_yco) {
                    max_yco = mmToPixel(layoutData.value(0).toDouble())+mmToPixel(4.0);
                }
            }
        }
    } else {
        top_yco = pr.y()+mmToPixel(15.0);
        max_yco = pr.height()-pr.y()-mmToPixel(5.0);
    }
    painter.begin(curr_printer);
    font.setFamily("Tahoma");
    painter.setFont(font);
    QPen pen(painter.pen());
    pen.setWidthF(mmToPixel(0.35));
    painter.setPen(pen);
    yco = top_yco;
    if (coverID>0) {
        printCustomPage(3,coverID);
        newPage();
    }
}

void Print::setPrinterFont(int size, bool bold, bool italic) {
    font.setPointSize(size);
    font.setBold(bold);
    font.setItalic(italic);
    painter.setFont(font);
    fontHeight = QFontMetricsF(painter.font()).height();
}

int Print::mmToPixel(double mm) {
    int px = ((double)curr_printer->width() * mm / ((double)curr_printer->widthMM()+10.0));
    return px;
}

void Print::drawStandardRow(QString plst, QString name, QString jg, QString verein, QString points, QString extra) {
    painter.drawText(QRectF(pr.x(), yco, mmToPixel(10.6), fontHeight),plst,QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(10.6), yco, mmToPixel(79.4), fontHeight),name,QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(79.4), yco, mmToPixel(10.6), fontHeight),jg,QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(90.0), yco, mmToPixel(79.4), fontHeight),verein,QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF(pr.x()+mmToPixel(155.0), yco, mmToPixel(45.0), fontHeight),extra,QTextOption(Qt::AlignVCenter));
    painter.drawText(QRectF((pr.width()-pr.x()-mmToPixel(15.9)), yco, mmToPixel(15.9), fontHeight),points,QTextOption(Qt::AlignVCenter | Qt::AlignRight));
    yco += fontHeight+mmToPixel(1.0);
}

QString Print::readDetailInfo(bool head, QString verein) {
    QString detail;
    if (head) {
        switch (detailinfo) {
        case 1: detail = "Turngau"; break;
        case 2: detail = "Verband"; break;
        case 3: detail = "Land"; break;
        default: detail = ""; break;
        }
    } else {
        if (verein.right(2) == "M.") verein = verein.left(verein.length()-8);
        QSqlQuery query;
        query.prepare("SELECT '',tfx_gaue.var_kuerzel, tfx_verbaende.var_kuerzel, tfx_laender.var_kuerzel FROM tfx_vereine INNER JOIN tfx_gaue ON tfx_vereine.int_gaueid = tfx_gaue.int_gaueid INNER JOIN tfx_verbaende ON tfx_verbaende.int_verbaendeid = tfx_gaue.int_verbaendeid INNER JOIN tfx_laender ON tfx_laender.int_laenderid = tfx_verbaende.int_laenderid WHERE tfx_vereine.var_name=?");
        query.bindValue(0,verein);
        query.exec();
        query.next();
        return query.value(detailinfo).toString();
    }
    return detail;
}

void Print::setTypeString(QString t) {
    typeString = t;
}

void Print::printCustomPage(int mode, int layoutid, QStringList tndata, QString tnwk) {
    QFont font2(font);
    Competition *competition = Competition::getByNumber(this->event, tnwk);
    QSqlQuery query2;
    if (tnwk == "") {
        query2.prepare("SELECT tfx_veranstaltungen.var_name, "+_global::date("dat_von",10)+", "+_global::date("dat_bis",10)+", tfx_wettkampforte.var_name, var_ort, '','' FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=?");
    } else {
        query2.prepare("SELECT tfx_veranstaltungen.var_name, "+_global::date("dat_von",10)+", "+_global::date("dat_bis",10)+", tfx_wettkampforte.var_name, var_ort, tfx_wettkaempfe.var_name, tfx_wettkaempfe.var_nummer FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) INNER JOIN tfx_wettkaempfe ON tfx_wettkaempfe.int_veranstaltungenid = tfx_veranstaltungen.int_veranstaltungenid WHERE tfx_veranstaltungen.int_veranstaltungenid=? AND tfx_wettkaempfe.var_nummer=?");
        query2.bindValue(1,tnwk);
    }
    query2.bindValue(0, this->event->mainEventId());
    query2.exec();
    query2.next();
    QSqlQuery layoutData;
    layoutData.prepare("SELECT int_typ, var_font, rel_x, rel_y, rel_w, rel_h, var_value, int_align FROM tfx_layout_felder WHERE int_layoutid=? ORDER BY int_layer");
    layoutData.bindValue(0,layoutid);
    layoutData.exec();

    while (layoutData.next()) {
        QFont fieldFont;
        fieldFont.fromString(layoutData.value(1).toString());
        int align;
        switch (layoutData.value(7).toInt()) {
        case 1: align = 0x0001; break;
        case 2: align = 0x0002; break;
        default: align = 0x0004; break;
        }
        if (layoutData.value(0).toInt() == 0) {
            QString field;
            QString verein;
            if (mode != 3 && mode != 2) {
                if (competition->getType() == 1) {
                    verein = tndata.at(1);
                } else {
                    verein = tndata.at(2);
                }
            }
            switch (layoutData.value(6).toInt()) {
            case 0 : field = query2.value(0).toString(); break;
            case 1 : {if (query2.value(1).toString() == query2.value(2).toString()) {
                        field = query2.value(1).toString();
                    } else {
                        field = query2.value(1).toString() + " - " + query2.value(2).toString();
                    }}; break;
            case 2 : field = query2.value(3).toString() + ", " + query2.value(4).toString(); break;
            case 3 : {if (tndata.size()>0) field = tndata.at(1);} break;
            case 4 : {if (tndata.size()>0) field = tndata.at(2);} break;
            case 5 : {if (tndata.size()>0) {
                        if (tndata.at(0).right(2) == " Q") {
                            field = tndata.at(0).left(tndata.at(0).length()-2) + ".";
                        } else {
                            field = tndata.at(0) + ".";
                        }
                    }
                }; break;
            case 6 : {
                    if (tndata.size()>0) {
                        bool isUrkunde=false;
                        Certificate *certificate = qobject_cast<Certificate*>(this);
                        if (certificate != nullptr) isUrkunde=true;
                        if (isUrkunde && certificate->getRundenErgebnisse() == true) {
                            field = tndata.at(tndata.size()-3);
                        } else {
                            field = tndata.at(tndata.size()-2);
                        }
                    }
                } break;
            case 7 : field = query2.value(5).toString(); break;
            case 8 : field = query2.value(5).toString() + _global::wkBez(this->event, query2.value(6).toString()); break;
            case 9 : if (tnwk != "") field = readDetailInfos(verein).at(0); break;
            case 10: if (tnwk != "") field = readDetailInfos(verein).at(1); break;
            case 11: if (tnwk != "") field = readDetailInfos(verein).at(2); break;
            case 12: field = typeString; break;
            case 13: {if (tndata.size()>0) {field = tndata.at(tndata.size()-2);}} break;
            case 14: {
                    if (tndata.size()>0) {
                        if (competition->getType() == 1 || competition->getType() == 2) {
                            QSqlQuery teamq;
                            if (competition->getType() == 1) {
                                teamq.prepare("SELECT " + _global::nameFormat() + " || CASE WHEN bol_ak='true' THEN ' (AK)' ELSE '' END FROM tfx_wertungen INNER JOIN tfx_teilnehmer USING (int_teilnehmerid) WHERE int_mannschaftenid=? AND int_runde=? ORDER BY int_mannschaftenid");
                            } else {
                                teamq.prepare("SELECT " + _global::nameFormat() + " FROM tfx_teilnehmer INNER JOIN tfx_gruppen_x_teilnehmer USING (int_teilnehmerid) INNER JOIN tfx_wertungen USING (int_gruppenid) WHERE int_wertungenid=? AND int_runde=? ORDER BY int_gruppenid");
                            }
                            teamq.bindValue(0, tndata.at(tndata.size()-1));
                            teamq.bindValue(1, this->event->round());
                            teamq.exec();
                            QStringList teams;
                            while (teamq.next()) {
                                teams.append(teamq.value(0).toString());
                            }
                            field = teams.join(", ");
                        }
                    }
                } break;
            case 15: field = tnwk; break;
            }
            painter.setFont(fieldFont);
            painter.drawText(mmToPixel(layoutData.value(2).toDouble()),mmToPixel(layoutData.value(3).toDouble()),mmToPixel(layoutData.value(4).toDouble()),mmToPixel(layoutData.value(5).toDouble()),align | Qt::TextWordWrap,field);
        } else if (layoutData.value(0).toInt() == 1) {
            painter.setFont(fieldFont);
            painter.drawText(mmToPixel(layoutData.value(2).toDouble()),mmToPixel(layoutData.value(3).toDouble()),mmToPixel(layoutData.value(4).toDouble()),mmToPixel(layoutData.value(5).toDouble()),align | Qt::TextWordWrap,layoutData.value(6).toString());
        } else if (layoutData.value(0).toInt() == 2) {
            if (!customImages.contains(layoutData.value(6).toString())) {
                QImage pm;
                pm.load(layoutData.value(6).toString());
                if (!pm.isNull()) {
                    customImages.insert(layoutData.value(6).toString(),pm.scaled(mmToPixel(layoutData.value(4).toDouble()),mmToPixel(layoutData.value(5).toDouble()),Qt::KeepAspectRatio,Qt::SmoothTransformation));
                }
            }
            painter.drawImage(mmToPixel(layoutData.value(2).toDouble()),mmToPixel(layoutData.value(3).toDouble()),customImages.value(layoutData.value(6).toString()));
        } else if (layoutData.value(0).toInt() == 3) {
            QPen pen;
            pen.setWidth(mmToPixel(layoutData.value(6).toDouble()));
            painter.setPen(pen);
            painter.drawLine(mmToPixel(layoutData.value(2).toDouble()),mmToPixel(layoutData.value(3).toDouble()),mmToPixel(layoutData.value(4).toDouble()+layoutData.value(2).toDouble()),mmToPixel(layoutData.value(5).toDouble()+layoutData.value(3).toDouble()));
            pen.setWidth(0);
            painter.setPen(pen);
        }
    }
    painter.setFont(font2);
}

QStringList Print::readDetailInfos(QString verein) {
    if (verein.right(2) == "M.") verein = verein.left(verein.length()-8);
    if (verein.right(10) == "Mannschaft") verein = verein.left(verein.length()-16);
    QSqlQuery query;
    query.prepare("SELECT tfx_gaue.var_name, tfx_verbaende.var_name, tfx_laender.var_name FROM tfx_vereine INNER JOIN tfx_gaue ON tfx_vereine.int_gaueid = tfx_gaue.int_gaueid INNER JOIN tfx_verbaende ON tfx_verbaende.int_verbaendeid = tfx_gaue.int_verbaendeid INNER JOIN tfx_laender ON tfx_laender.int_laenderid = tfx_verbaende.int_laenderid WHERE tfx_vereine.var_name=?");
    query.bindValue(0,verein);
    query.exec();
    query.next();
    QStringList infos;
    infos << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
    return infos;
}

void Print::drawHighlightRect(qreal y, qreal h) {
    if (h==-1) h = QFontMetricsF(painter.font()).height() + mmToPixel(1.0);
    QRectF r;
    painter.setPen(QPen(QColor(240,240,240,255)));
    r.setRect(pr.x(),y, (pr.width()-pr.x()-pr.x()), h);
    painter.fillRect(r,QBrush(QColor(240,240,240,255)));
    painter.drawRect(r);
    painter.setPen(QPen(QColor(0,0,0,255)));
}

void Print::drawTextLine(QString text, int x, bool newLine) {
    if (x == 0) {
        x = pr.x();
    }
    painter.drawText(QRectF(x, yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),text);
    if (newLine) yco += mmToPixel(5.3);
}
