#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "results.h"

class Individual : public Results {
    Q_OBJECT

public:
    Individual(Event *event) : Results(event) {}
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // EINZEL_H
