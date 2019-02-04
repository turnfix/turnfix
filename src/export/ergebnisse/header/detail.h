#ifndef DETAIL_H
#define DETAIL_H

#include "ergebnisse.h"

class Detail : public Ergebnisse {
    Q_OBJECT

public:
    virtual void printContent();
    virtual void printSubHeader();
    static void setPrintAW(bool);

private:
    static bool printAW;

    struct resultData {
        QString disname;
        QString result;
        QString detail;
        QString aw;
    };
    QMap<int, QMap<int, resultData> > results;

    struct position {
        int x;
        int y;
    };

};

#endif // DETAIL_H
