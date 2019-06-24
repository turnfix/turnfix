#include "results.h"
#include "src/global/header/_global.h"

bool Results::newPageWK = false;

void Results::print(QPrinter *printer) {
    Print::print(printer);
    printHeadFoot();
    printContent();
}

void Results::setnewPageWK(bool set) {
    newPageWK = set;
}

bool Results::checkFitPage(int h, QString wk, bool f) {
    if (yco+h > max_yco) {
        newPage();
        printDescriptor(wk,f);
        return false;
    }
    return true;
}
