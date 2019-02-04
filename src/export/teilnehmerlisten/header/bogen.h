#ifndef BOGEN_H
#define BOGEN_H

#include "liste.h"

class Bogen : public Liste {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();
    static void setTeammode(bool set);

private:
    QString currRiege;
    int currDis;
    static bool teammode;
    void printType(QString,QString);

};

#endif // BOGEN_H
