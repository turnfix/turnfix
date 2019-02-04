#ifndef ERGEBNISSE_H
#define ERGEBNISSE_H

#include "../../header/drucken.h"

class Ergebnisse : public Drucken {
    Q_OBJECT
public:
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

#endif // ERGEBNISSE_H
