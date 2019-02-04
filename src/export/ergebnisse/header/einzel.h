#ifndef EINZEL_H
#define EINZEL_H

#include "ergebnisse.h"

class Einzel : public Ergebnisse {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // EINZEL_H
