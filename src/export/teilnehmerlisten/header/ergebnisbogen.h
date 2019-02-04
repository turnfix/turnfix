#ifndef ERGEBNISBOGEN_H
#define ERGEBNISBOGEN_H

#include "liste.h"

class ErgebnisBogen : public Liste {

Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();

private:
    QString currRiege;
    int currDis;
    void printType(QString,QString);

};

#endif // BOGEN_H
