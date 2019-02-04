#ifndef BERICHT_H
#define BERICHT_H

#include "../../header/drucken.h"

class Bericht : public Drucken {
    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // BERICHT_H
