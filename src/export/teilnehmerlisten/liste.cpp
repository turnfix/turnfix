#include "header/liste.h"
#include "../../global/header/_global.h"

void Liste::print(QPrinter *printer) {
    Drucken::print(printer);
    printHeadFoot();
    printContent();
}

void Liste::printSubHeader() {
    setPrinterFont(10);
    QString jg = "";
    if (_global::checkTyp(currWK)==0) jg = "Jg.";

    if (_global::checkTyp(currWK)==0 || _global::checkTyp(currWK)==2) {
        drawStandardRow("StNr.","Name",jg,"Verein","",readDetailInfo(true));
    } else {
        drawStandardRow("StNr.","Name","Jg.","Mannschaft","",readDetailInfo(true));
    }
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += 1;
}

bool Liste::checkWKChange(QString currWK, QString lastWK, double lineHeight, bool newPageCreated) {
    int skip=0;
    if (lastWK != currWK) {
        skip += mmToPixel(27.8);
    }
    skip += mmToPixel(lineHeight);
    if (yco+skip > max_yco) {
        newPage();
        newPageCreated = true;
        if (lastWK == currWK) printDescriptor(currWK,1);
    }
    if (lastWK != currWK) {
        if (lastWK != "" && !newPageCreated) yco += mmToPixel(5.0);
        printDescriptor(currWK);
    }
    return newPageCreated;
}
