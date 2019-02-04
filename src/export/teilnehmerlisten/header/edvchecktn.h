#ifndef EDVCHECKTN_H
#define EDVCHECKTN_H

#include "liste.h"

class EDVCheckTN : public Liste {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // EDVCHECKTN_H
