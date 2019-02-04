#ifndef MELDUNG_H
#define MELDUNG_H

#include "liste.h"

class Meldung : public Liste {
    Q_OBJECT

public:
    virtual void printContent();

};

#endif // MELDUNG_H
