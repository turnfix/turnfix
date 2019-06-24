#ifndef ROUND_H
#define ROUND_H

#include "results.h"

class Round : public Results {
    Q_OBJECT

public:
    Round(Event *event) : Results(event) {}
    virtual void printContent();
    virtual void printSubHeader();
    static void setUseExtraScore(bool);

private:
    static bool useExtraScore;

};

#endif // ROUND_H
