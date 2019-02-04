#ifndef RUNDE_H
#define RUNDE_H

#include "ergebnisse.h"

class Runde : public Ergebnisse {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();
    static void setUseExtraScore(bool);

private:
    static bool useExtraScore;

};

#endif // RUNDE_H
