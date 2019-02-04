#ifndef LISTE_H
#define LISTE_H

#include "../../header/drucken.h"

class Liste : public Drucken {
    Q_OBJECT

public:
    virtual void printSubHeader();

public slots:
    virtual void print(QPrinter*);

protected:
    bool checkWKChange(QString currWK,QString lastWK, double lineHeight, bool newPageCreated=false);
    QString currWK;

};

#endif // LISTE_H
