#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <QObject>
#include <QFont>
#include <QVariant>
#include <QStringList>

class QSqlQuery;

class _global : public QObject {
    Q_OBJECT

public:
    static QString wkBez(QString swknr);
    static void setWkVars(int wknr, int runde, int hwk);
    static void setDBTyp(int typ);
    static int checkHWK();
    static int getRunde();
    static int getWkNr();
    static int getDBTyp();
    static void initFields();
    static QStringList getFields();
    static int checkTyp(QString swknr);
    static bool checkRoundWK();
    static void updateRgDis();
    static QString nameFormat();
    static double calcLeistung(QString value);
    static QList<QVariant> nameSplit(QString name);
    static QString strLeistung(double lst, QString einheit, QString maske, int nk);
    static QList<QStringList> createStartlist(QString riege,int dis,int bahnen);
    static QList<int> splitColorArray(QString array);
    static QString intListToString(QList<int>);
    static int querySize(QSqlQuery query);
    static QString substring(QString field, QString from);
    static QString date(QString field, int length);
    struct itemdata {
        int id;
        int typ;
        double x;
        double y;
        double width;
        double height;
        QFont font;
        QVariant value;
        int align;
    };
private:
    static int wknr;
    static int runde;
    static int hwk;
    static int dbtyp;
    static QStringList fields;
};

#endif
