#include "list.h"
#include "model/objects/competition.h"
#include "src/global/header/_global.h"

void List::print(QPrinter *printer) {
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void List::printSubHeader() {
    Competition *competition = Competition::getByNumber(this->event, currWK);

    setPrinterFont(10);
    QString jg = "";

    if (competition->getType() == 0) jg = "Jg.";

    if (competition->getType() == 0 || competition->getType() == 2) {
        drawStandardRow("StNr.","Name",jg,"Verein","",readDetailInfo(true));
    } else {
        drawStandardRow("StNr.","Name","Jg.","Mannschaft","",readDetailInfo(true));
    }
    painter.drawLine(QPointF(pr.x(),yco),QPointF(pr.width()-pr.x(),yco));
    yco += 1;
}

bool List::checkWKChange(QString currWK, QString lastWK, double lineHeight, bool newPageCreated) {
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
