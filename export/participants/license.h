#ifndef LICENSE_H
#define LICENSE_H

#include "list.h"

class License : public List {
    Q_OBJECT

public:
    License(Event *event) : List(event) {}
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // LICENSE_H
