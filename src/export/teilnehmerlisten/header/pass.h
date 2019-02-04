#ifndef PASS_H
#define PASS_H

#include "liste.h"

class Pass : public Liste {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // PASS_H
