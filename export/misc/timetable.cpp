#include <QTime>
#include "timetable.h"
#include "src/global/header/_global.h"

void Timetable::print(QPrinter *printer) {
    setTypeString("Zeitplan");
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void Timetable::printContent() {
    QSqlQuery query;
    query.prepare("SELECT int_durchgang, int_bahn, var_nummer, tim_einturnen, tim_startzeit, var_name FROM tfx_wettkaempfe WHERE int_veranstaltungenid=? ORDER BY int_durchgang,int_bahn,var_nummer");
    query.bindValue(0, this->event->getMainEventId());
    query.exec();
    int max_y;
    int start_y;
    int lastBahn;
    int lastDurchgang;
    QTime lastStart;
    QTime lastWarmUp;
    while (query.next()) {
        if (query.value(0).toInt() != lastDurchgang) {
            lastDurchgang = query.value(0).toInt();
            lastBahn = 0;
            lastStart = QTime();
            lastWarmUp = QTime();
            if (query.at() > 0) yco = max_y + mmToPixel(15.9);
            setPrinterFont(14,true);
            painter.drawText(QRectF(pr.x(), yco, pr.width()-pr.x()-pr.x(), QFontMetricsF(painter.font()).height()),query.value(0).toString() + ". Durchgang",QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
            start_y = yco + mmToPixel(6.6);
        }
        if (query.value(1).toInt() != lastBahn) {
            lastBahn = query.value(1).toInt();
            yco = start_y;
            lastStart = QTime();
            lastWarmUp = QTime();
            setPrinterFont(12,true);
            painter.drawText(QRectF(pr.x()+(query.value(1).toInt()-1)*((pr.width()-pr.x()-pr.x())/3), yco, ((pr.width()-pr.x()-pr.x())/3), QFontMetricsF(painter.font()).height()),"Bahn " + query.value(1).toString(),QTextOption(Qt::AlignVCenter | Qt::AlignCenter));
            yco += mmToPixel(4.8);
        }
        if (QTime().fromString(query.value(3).toString(),"hh:mm:ss") != lastWarmUp) {
            lastWarmUp = QTime().fromString(query.value(3).toString(),"hh:mm:ss");
            setPrinterFont(11,true);
            yco += mmToPixel(1.3);
            painter.drawText(QRectF(pr.x()+(query.value(1).toInt()-1)*((pr.width()-pr.x()-pr.x())/3), yco, ((pr.width()-pr.x()-pr.x())/3), QFontMetricsF(painter.font()).height()),"Einturnen: " + lastWarmUp.toString("hh:mm") + " Uhr",QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
            yco += mmToPixel(4.2);
        }
        if (QTime().fromString(query.value(4).toString(),"hh:mm:ss") != lastStart) {
            lastStart = QTime().fromString(query.value(4).toString(),"hh:mm:ss");
            setPrinterFont(11,true);
            painter.drawText(QRectF(pr.x()+(query.value(1).toInt()-1)*((pr.width()-pr.x()-pr.x())/3), yco, ((pr.width()-pr.x()-pr.x())/3), QFontMetricsF(painter.font()).height()),"Beginn: " + lastStart.toString("hh:mm") + " Uhr",QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
            yco += mmToPixel(4.2);
        }
        setPrinterFont(11);
        painter.drawText(QRectF(pr.x()+(query.value(1).toInt()-1)*((pr.width()-pr.x()-pr.x())/3), yco, ((pr.width()-pr.x()-pr.x())/3), QFontMetricsF(painter.font()).height()),"Wk: " + query.value(2).toString() + " " + query.value(5).toString() + " " + _global::wkBez(this->event, query.value(2).toString()),QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
        yco += mmToPixel(4.2);
        max_y = yco;
    }
    finishPrint();
}
