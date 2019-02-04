#ifndef RIEGE_H
#define RIEGE_H

#include "liste.h"

class Riege : public Liste {
    Q_OBJECT

public:
    virtual void printContent();
    static void setOrder(int);
    static void setNewPageEach(bool);

private:
    static int order;
    static bool newPageEach;
    QString currRiege;

};

#endif // RIEGE_H
