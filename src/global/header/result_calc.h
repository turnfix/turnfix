#ifndef RESULT_CALC_H
#define RESULT_CALC_H

#include <QObject>

class Result_Calc : public QObject {
    Q_OBJECT

public:
    static QList<QStringList> resultArrayNew(QString swknr="", int typ=-1, QList<int> cres=QList<int>(), int rnd=-1, int print=1, bool printAW=false,QString detailQuery="");
    static QList<QStringList> roundResultArrayNew(QString swknr="",bool useExtraScore=false,QString detailQuery="");
    static QList<QStringList> tabllenArray(QString swknr);
    static QList<QStringList> sortRes(QList<QStringList>,int mode=0);
    static QList<QStringList> quicksort(QList<QStringList> l, int left, int right, int mode);


};

#endif // RESULT_CALC_H
