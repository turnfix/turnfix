#ifndef RESULTSSHEET_H
#define RESULTSSHEET_H

#include "list.h"

class ResultsSheet : public List {

Q_OBJECT

public:
    ResultsSheet(Event *event) : List(event) {}
    virtual void printContent();
    virtual void printSubHeader();

private:
    QString currRiege;
    int currDis;
    void printType(QString,QString);

};

#endif // RESULTSSHEET_H
