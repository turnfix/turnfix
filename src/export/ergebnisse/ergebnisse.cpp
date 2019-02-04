#include "header/ergebnisse.h"
#include "../../global/header/_global.h"

bool Ergebnisse::newPageWK = false;

void Ergebnisse::print(QPrinter *printer) {
    Drucken::print(printer);
    printHeadFoot();
    printContent();
}

void Ergebnisse::setnewPageWK(bool set) {
    newPageWK = set;
}

bool Ergebnisse::checkFitPage(int h, QString wk, bool f) {
    if (yco+h > max_yco) {
        newPage();
        printDescriptor(wk,f);
        return false;
    }
    return true;
}
