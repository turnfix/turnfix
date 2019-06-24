#include <math.h>
#include <QDate>
#include "invitation.h"
#include "src/global/header/_global.h"

void Invitation::print(QPrinter *printer) {
    setTypeString("Ausschreibung");
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void Invitation::printContent() {
    setPrinterFont(20,true);
    QSqlQuery query;
    query.prepare("SELECT int_veranstaltungenid, int_wettkampforteid, int_meldung_an, int_ansprechpartner, tfx_konten.int_kontenid, int_hauptwettkampf, tfx_veranstaltungen.var_name, int_runde, dat_von, dat_bis, dat_meldeschluss, bol_rundenwettkampf, var_veranstalter, int_edv, int_helfer, int_kampfrichter, var_meldung_website, var_verwendungszweck, rel_meldegeld, rel_nachmeldung, bol_faellig_nichtantritt, bol_ummeldung_moeglich, bol_nachmeldung_moeglich, txt_meldung_an, txt_startberechtigung, txt_teilnahmebedingungen, txt_siegerauszeichnung, txt_kampfrichter, txt_hinweise, tfx_wettkampforte.var_name, tfx_wettkampforte.var_adresse, tfx_wettkampforte.var_plz, tfx_wettkampforte.var_ort, int_personenid, var_vorname, var_nachname, tfx_personen.var_adresse, tfx_personen.var_plz, tfx_personen.var_ort, var_telefon, var_fax, var_email, tfx_konten.var_name, var_kontonummer, var_blz, var_bank, var_inhabe FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) LEFT JOIN tfx_personen ON int_meldung_an = tfx_personen.int_personenid LEFT JOIN tfx_konten ON tfx_konten.int_kontenid = tfx_veranstaltungen.int_kontenid WHERE int_veranstaltungenid=?");
    query.bindValue(0, this->event->getMainEventId());
    query.exec();
    query.next();
    QFontMetricsF fm(painter.font());
    qreal pixelsWide = fm.width(query.value(6).toString());
    qreal pixelsHigh = fm.height();
    double boxwide = ceil(pixelsWide/(pr.width()-pr.x()-pr.x()));
    QTextOption op;
    op.setWrapMode(QTextOption::WordWrap);
    op.setAlignment(Qt::AlignHCenter);
    painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), pixelsHigh*boxwide),query.value(6).toString(),op);
    double plus = pixelsHigh*boxwide;
    yco += QVariant(plus).toInt() + mmToPixel(5.3);
    drawHeader("Veranstalter");
    drawTextLine(query.value(12).toString());
    yco += mmToPixel(2.1);
    drawHeader("Wettkampfort & Datum");
    if (query.value(11).toBool()) {
        QSqlQuery runden;
        runden.prepare("SELECT int_veranstaltungenid, int_runde FROM tfx_veranstaltungen WHERE int_veranstaltungenid=? OR int_hauptwettkampf=? ORDER BY int_runde");
        runden.bindValue(0,query.value(0).toInt());
        runden.bindValue(1,query.value(0).toInt());
        runden.exec();
        int xco = pr.x();
        int ystart = yco;
        while (runden.next()) {
            QSqlQuery rdet;
            rdet.prepare("SELECT tfx_wettkampforte.var_name, var_adresse, var_plz, var_ort, dat_von, dat_bis FROM tfx_veranstaltungen INNER JOIN tfx_wettkampforte USING (int_wettkampforteid) WHERE int_veranstaltungenid=?");
            rdet.bindValue(0,runden.value(0).toInt());
            rdet.exec();
            rdet.next();
            setPrinterFont(12,true);
            yco = ystart;
            drawTextLine(runden.value(1).toString() + ". Runde", xco);
            setPrinterFont(12,false,true);
            if (rdet.value(4).toString() == rdet.value(5).toString()) {
                drawTextLine(QDate().fromString(rdet.value(4).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy"), xco);
            } else {
                drawTextLine( QDate().fromString(rdet.value(4).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy") + " - " + QDate().fromString(rdet.value(5).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy"), xco);
            }
            setPrinterFont(12);
            drawTextLine(rdet.value(0).toString(), xco);
            drawTextLine(rdet.value(1).toString(), xco);
            drawTextLine(rdet.value(2).toString() + " " + rdet.value(3).toString(), xco);
            QSqlQuery beginn;
            beginn.prepare("SELECT tim_startzeit FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY tim_startzeit LIMIT 1");
            beginn.bindValue(0,query.value(0).toString());
            beginn.exec();
            beginn.next();
            if (beginn.value(0).toString() != "" && beginn.value(0).toString() != "00:00:00") {
                drawTextLine("", xco);
                drawTextLine("Beginn: " + beginn.value(0).toString().left(5) + " Uhr", xco);
            }
            xco = xco + mmToPixel(47.7);

        }
    } else {
        setPrinterFont(12,false,true);
        if (query.value(8).toString() == query.value(9).toString()) {
            drawTextLine(QDate().fromString(query.value(8).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy"));
        } else {
            drawTextLine( QDate().fromString(query.value(8).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy") + " - " + QDate().fromString(query.value(9).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy"));
        }
        setPrinterFont(12);
        drawTextLine(query.value(29).toString());
        drawTextLine(query.value(30).toString());
        drawTextLine(query.value(31).toString() + " " + query.value(32).toString());
        QSqlQuery beginn;
        beginn.prepare("SELECT tim_startzeit FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY tim_startzeit LIMIT 1");
        beginn.bindValue(0,query.value(0).toString());
        beginn.exec();
        beginn.next();
        QString startzeit;
        if (beginn.value(0).toString() != "" && beginn.value(0).toString() != "00:00:00") {
            drawTextLine("");
            drawTextLine("Beginn: " + beginn.value(0).toString().left(5) + " Uhr");
        }
    }
    yco += mmToPixel(2.1);
    drawHeader("Wettkämpfe");
    QSqlQuery query3;
    query3.prepare("SELECT int_wettkaempfeid, var_nummer, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY var_nummer");
    query3.bindValue(0, this->event->getMainEventId());
    query3.exec();
    while (query3.next()) {
        QSqlQuery query2;
        query2.prepare("SELECT tfx_disziplinen.var_name, var_ausschreibung FROM tfx_wettkaempfe_x_disziplinen INNER JOIN tfx_disziplinen USING (int_disziplinenid) WHERE tfx_wettkaempfe = ? ORDER BY int_sortierung");
        query2.bindValue(0, query3.value(0).toString());
        query2.exec();
        checkNewPage((yco + mmToPixel(1.3) + (ceil(_global::querySize(query2)/2))*mmToPixel(4.5) + mmToPixel(5.3)));
        setPrinterFont(12,true);
        painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),"WK Nr. " + query3.value(1).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.x()+mmToPixel(35.0), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),query3.value(2).toString(),QTextOption(Qt::AlignVCenter));
        painter.drawText(QRectF(pr.width()/2, yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),_global::wkBez(this->event, query3.value(1).toString()),QTextOption(Qt::AlignVCenter));
        yco += mmToPixel(1.3);
        int i=0;
        setPrinterFont(9, false,true);
        int x;
        while (query2.next()) {
            if (i%2 == 0) {
                yco += mmToPixel(4.5);
                x = pr.x();
            } else {
                x = pr.width()/2;
            }
            QString extra;
            if (query2.value(1).toString().length()>0) {
                extra = " (" + query2.value(1).toString() + ")";
            } else {
                extra = "";
            }
            painter.drawText(QRectF(x, yco, (pr.width()-pr.x()-pr.x())/2, QFontMetricsF(painter.font()).height()), query2.value(0).toString()+extra,QTextOption(Qt::AlignVCenter));
            i++;
        }
        yco += mmToPixel(5.3);
        setPrinterFont(11);
    }
    yco += mmToPixel(2.6);
    drawUserText(query.value(24).toString(),"Startberechtigung - Nachweis");
    drawUserText(query.value(25).toString(),"Teilnahmebedingungen");
    QString meldeadresse = query.value(34).toString() + " " + query.value(35).toString() + "\n" + query.value(36).toString() + "\n" + query.value(37).toString() + " " + query.value(38).toString();
    if (query.value(39).toString().length() > 0) meldeadresse = meldeadresse + "\nTel.: " + query.value(39).toString();
    if (query.value(40).toString().length() > 0) meldeadresse = meldeadresse + "\nFax: " + query.value(40).toString();
    if (query.value(41).toString().length() > 0) meldeadresse = meldeadresse + "\nEmail: " + query.value(41).toString();
    meldeadresse = meldeadresse + "\n\n" + query.value(26).toString();
    drawUserText(meldeadresse,"Meldung an");
    checkNewPage(mmToPixel(25.4));
    drawHeader("Meldeschluss");
    drawTextLine(QDate().fromString(query.value(10).toString(),"yyyy-MM-dd").toString("dd.MM.yyyy"));
    if (!query.value(21).toBool() && !query.value(22).toBool()) {
        drawTextLine("Nach Meldeschluss sind keine Nach- und Ummeldungen möglich.");
    } else if (!query.value(21).toBool() && query.value(22).toBool()) {
        drawTextLine("Nach Meldeschluss sind Ummeldungen möglich.");
    } else if (query.value(21).toBool() && !query.value(22).toBool()) {
        drawTextLine("Nach Meldeschluss sind Nachmeldungen möglich.");
        drawTextLine("(Zusätzliche Nachmeldegebühr: " +  QString().setNum(query.value(19).toDouble(),'f',2) +  " Euro)");
    } else {
        drawTextLine("Nach Meldeschluss sind Ummeldungen und Nachmeldungen möglich.");
        drawTextLine("(Zusätzliche Nachmeldegebühr: " +  QString().setNum(query.value(19).toDouble(),'f',2) +  " Euro)");
    }
    yco += mmToPixel(2.1);
    QString meldegeld = query.value(18).toString() + " Euro pro Teilnehmer/Mannschaft/Gruppe";
    if (query.value(20).toBool()) {
        meldegeld = meldegeld + "\nDas Meldegeld wird auch bei Nichtantritt fällig";
    }
    if (query.value(42).toString().length() > 0 && query.value(43).toString().length() > 0 && query.value(44).toString().length() > 0 && query.value(45).toString().length() > 0) {
        meldegeld = meldegeld + "\n\nDas Meldegeld ist vorab auf folgendes Konto zu überweisen:\nInhaber: " + query.value(42).toString() + "\nKonto-Nr.: " + query.value(43).toString() + "\nBLZ: " + query.value(44).toString() + "\nBank: " + query.value(45).toString();
    }
    if (query.value(17).toString().length() > 0) {
        meldegeld = meldegeld + "\nVerwendungszweck: " + query.value(45).toString();
    }
    drawUserText(meldegeld,"Meldegeld");
    drawUserText(query.value(26).toString(),"Sieger-Auszeichnung");
    drawUserText(query.value(27).toString(),"Kampfrichter/innen");
    drawUserText(query.value(28).toString(),"Hinweise");
    finishPrint();
}

void Invitation::drawHeader(QString text) {
    setPrinterFont(13,true);
    painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),text);
    yco += mmToPixel(5.8);
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += mmToPixel(1.3);
    setPrinterFont(11);
}

void Invitation::drawUserText(QString text, QString header) {
    double plus;
    QTextOption op;
    QFontMetricsF fm2(painter.font());
    op.setWrapMode(QTextOption::WordWrap);
    plus = fm2.boundingRect(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), mmToPixel(2.6)),Qt::TextWordWrap,text).height();
    checkNewPage((yco + mmToPixel(7.9) + plus));
    drawHeader(header);
    painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), QVariant(plus).toInt()),text,op);
    yco += mmToPixel(2.1) + QVariant(plus).toInt();
}

void Invitation::checkNewPage(double plus) {
    if (plus > max_yco) {
        newPage();
    }
}
