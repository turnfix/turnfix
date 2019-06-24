#ifndef ITCHECKSQUADS_H
#define ITCHECKSQUADS_H

#include "../print.h"

class ITCheckSquads : public Print {
    Q_OBJECT

public:
    ITCheckSquads(Event *event) : Print(event) {}

    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // ITCHECKSQUADS_H
