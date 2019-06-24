#ifndef ITCHECKLIST_H
#define ITCHECKLIST_H

#include "list.h"

class ITCheckList : public List {
    Q_OBJECT

public:
    ITCheckList(Event *event) : List(event) {}
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // ITCHECKLIST_H
