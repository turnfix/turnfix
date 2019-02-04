#ifndef URKUNDE_H
#define URKUNDE_H

#include "../../header/drucken.h"

class Urkunde : public Drucken {

    Q_OBJECT
public:
    virtual void print(QPrinter*);
    virtual void printContent();
    static void setRundenErgebnisse(bool);
    static void setEineUrkunde(bool);
    static void setPlatzWertung(bool);
    static void setEinzelErgebnis(bool);
    static void setUrkundenID(int);
    static bool getRundenErgebnisse();

private:
    static bool rundenErgebnisse;
    static bool eineUrkunde;
    static bool platzWertung;
    static bool einzelErgebnis;
    static int urkundenID;

};

#endif // URKUNDE_H
