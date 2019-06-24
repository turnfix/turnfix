#ifndef MEDALCOUNT_H
#define MEDALCOUNT_H

#include "../print.h"

class MedalCount : public Print {
    Q_OBJECT

public:
    MedalCount(Event *event) : Print(event) {}

    virtual void print(QPrinter*);
    virtual void printContent();
    virtual void printSubHeader();

private:
    QList<int> ids;
    QMap<int,int> gesamt;
    QMap<int,int> gold;
    QMap<int,int> silber;
    QMap<int,int> bronze;
    QMap<int,QString> namen;

};

#endif // MEDALCOUNT_H
