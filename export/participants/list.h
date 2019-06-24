#ifndef LISTE_H
#define LISTE_H

#include "../print.h"

class List : public Print {
    Q_OBJECT

public:
    List(Event *event) : Print(event) {}
    virtual void printSubHeader();

public slots:
    virtual void print(QPrinter*);

protected:
    bool checkWKChange(QString currWK,QString lastWK, double lineHeight, bool newPageCreated=false);
    QString currWK;

};

#endif // LISTE_H
