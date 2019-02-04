#ifndef AUSSCHREIBUNG_H
#define AUSSCHREIBUNG_H

#include "../../header/drucken.h"

class Ausschreibung : public Drucken {

    Q_OBJECT

public:
    virtual void print(QPrinter*);
    virtual void printContent();

private:
    void drawHeader(QString text);
    void drawUserText(QString text, QString header);
    void checkNewPage(double plus);

};

#endif // AUSSCHREIBUNG_H
