#ifndef EDVCHECKRIEGE_H
#define EDVCHECKRIEGE_H

#include "../../header/drucken.h"

class EDVCheckRiege : public Drucken {

    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // EDVCHECKRIEGE_H
