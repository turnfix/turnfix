#ifndef KARTE_H
#define KARTE_H

#include "../../header/drucken.h"

class Karte : public Drucken {
    Q_OBJECT

public slots:
    virtual void print(QPrinter*);
    virtual void printContent();

};

#endif // KARTE_H
