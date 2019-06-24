#ifndef TABLE_H
#define TABLE_H

#include "results.h"

class Table : public Results {
    Q_OBJECT

public:
    Table(Event *event) : Results(event) {}
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // TABLE_H
