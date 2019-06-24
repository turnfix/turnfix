#ifndef REGISTRATIONMATRIX_H
#define REGISTRATIONMATRIX_H

#include "../print.h"

class RegistrationMatrix : public Print {

    Q_OBJECT

public:
    RegistrationMatrix(Event *event) : Print(event) {}
    virtual void print(QPrinter*);
    virtual void printContent();
    static void setTeamMode(bool teamMode);
private:
    static bool teamMode;

};

#endif // REGISTRATIONMATRIX_H
