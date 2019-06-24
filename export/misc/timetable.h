#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "../print.h"

class Timetable : public Print {
    Q_OBJECT

public:
    Timetable(Event *event) : Print(event) {}
    virtual void print(QPrinter*);
    virtual void printContent();
};

#endif // TIMETABLE_H
