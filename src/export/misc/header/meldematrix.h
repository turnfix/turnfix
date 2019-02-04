#ifndef MELDEMATRIX_H
#define MELDEMATRIX_H

#include "../../header/drucken.h"

class MeldeMatrix : public Drucken {

    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();
    static void setTeamMode(bool teamMode);
private:
    static bool teamMode;

};

#endif // MELDEMATRIX_H
