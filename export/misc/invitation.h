#ifndef INVITATION_H
#define INVITATION_H

#include "../print.h"

class Invitation : public Print {
    Q_OBJECT

public:
    Invitation(Event *event) : Print(event) {}

    virtual void print(QPrinter*);
    virtual void printContent();

private:
    void drawHeader(QString text);
    void drawUserText(QString text, QString header);
    void checkNewPage(double plus);

};

#endif // INVITATION_H
