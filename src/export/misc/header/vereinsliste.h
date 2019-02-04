#ifndef VEREINSLISTE_H
#define VEREINSLISTE_H

#include "../../header/drucken.h"

class Vereinsliste : public Drucken {
    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // VEREINSLISTE_H
