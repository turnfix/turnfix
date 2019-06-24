#ifndef CARD_H
#define CARD_H

#include "../print.h"

class Card : public Print {
    Q_OBJECT

public:
    Card(Event *event) : Print(event) {}

public slots:
    virtual void print(QPrinter*);
    virtual void printContent();

};

#endif // CARD_H
