#ifndef MEDALLIENSPIEGEL_H
#define MEDALLIENSPIEGEL_H

#include "../../header/drucken.h"

class Medallienspiegel : public Drucken {
    Q_OBJECT

public:
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

#endif // MEDALLIENSPIEGEL_H
