#ifndef BERICHT_H
#define BERICHT_H

#include "../print.h"

class Summary : public Print {
    Q_OBJECT

public:
    Summary(Event *event) : Print(event) {}

    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // BERICHT_H
