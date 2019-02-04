#ifndef ZEITPLAN_H
#define ZEITPLAN_H

#include "../../header/drucken.h"

class Zeitplan : public Drucken {

    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // ZEITPLAN_H
