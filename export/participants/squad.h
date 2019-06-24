#ifndef SQUAD_H
#define SQUAD_H

#include "list.h"

class Squad : public List {
    Q_OBJECT

public:
    Squad(Event *event) : List(event) {}
    virtual void printContent();
    static void setOrder(int);
    static void setNewPageEach(bool);

private:
    static int order;
    static bool newPageEach;
    QString currRiege;

};

#endif // SQUAD_H
