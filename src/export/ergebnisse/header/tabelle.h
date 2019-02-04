#ifndef TABELLE_H
#define TABELLE_H

#include "ergebnisse.h"

class Tabelle : public Ergebnisse {

    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // TABELLE_H
