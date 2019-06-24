#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "../print.h"

class Certificate : public Print {

    Q_OBJECT
public:
    Certificate(Event *event) : Print(event) {}

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

#endif // CERTIFICATE_H
