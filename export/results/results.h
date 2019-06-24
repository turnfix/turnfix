#ifndef RESULTS_H
#define RESULTS_H

#include "../print.h"

class Results : public Print {
    Q_OBJECT
public:
    Results(Event *event) : Print(event) {}
    static void setnewPageWK(bool);

public slots:
    virtual void print(QPrinter*);

protected:
    static bool newPageWK;
    QString currWK;
    bool currWahl;
    bool currKP;
    QSqlQuery wkQuery;
    bool checkFitPage(int,QString,bool=false);
};

#endif // RESULTS_H
