#ifndef CLUBLIST_H
#define CLUBLIST_H

#include "../print.h"

class ClubList : public Print {
    Q_OBJECT

public:
    ClubList(Event *event) : Print(event) {}

    virtual void print(QPrinter*);
    virtual void printContent();
    virtual void printSubHeader();

};

#endif // CLUBLIST_H
