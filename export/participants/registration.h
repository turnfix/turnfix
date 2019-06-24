#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "list.h"

class Registration : public List {
    Q_OBJECT

public:
    Registration(Event *event) : List(event) {}
    virtual void printContent();

};

#endif // REGISTRATION_H
