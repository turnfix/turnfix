#ifndef DRUCKEN_H
#define DRUCKEN_H

#include "model/entity/event.h"
#include <QMap>
#include <QPainter>
#include <QPrinter>
#include <QSqlQuery>
#include <QThread>

class QFont;
class QRect;

class Print : public QThread {
    Q_OBJECT

public:
    Print(Event *event);
    void setOutputType(int);
    int  getOutputType();
    static void setDetailInfo(int);
    void setShowPreview(bool);
    void setSelectClub(bool);
    void setSelectRiege(bool);
    void setSelectTN(bool);
    void setSelectWK(bool);
    void setSelectDis(bool);
    void setSelectDetail(bool);
    void setTypeString(QString);
    static void setHeadFootID(int);
    static void setCoverID(int);
    void setOutputFileName(QString);
    static void setPaperSize(QPrinter::PaperSize);
    static QPrinter::PaperSize getPaperSize();
    static void setOrientation(QPrinter::Orientation);
    static QPrinter::Orientation getOrientation();
    void setWKNumber(QString);
    void setVerein(int);
    void setFinish(bool);

    void setDetailQuery(QString);
    void setSelectedTNWK(QString);
    void setSelectedClubs(QList<int>);
    void setSelectedTN(QList<int>);
    void setTeilnehmerNumbers(QList<int>);
    void setSelectedWKs(QStringList);
    void setSelectedRiegen(QStringList);
    void setSelectedDisziplinen(QList< QList<int> >);


public slots:
    virtual void print(QPrinter*);

protected:
    void run();
    QPrinter *curr_printer;
    QPainter painter;
    QRect pr;
    QFont font;
    QString typeString;

    Event *event;

    static int detailinfo;
    bool korr;
    int yco;
    int max_yco;
    int top_yco;
    qreal fontHeight;
    void printHeadFoot();
    void printDescriptor(QString,bool=false);
    virtual void printContent();
    virtual void printSubHeader();
    void newPage(bool printHeadFoot = true);
    void finishPrint();
    int mmToPixel(double mm);
    void setPrinterFont(int size, bool bold=false, bool italic=false);
    void drawStandardRow(QString plst, QString name, QString jg, QString verein, QString points="", QString extra="");
    QString readDetailInfo(bool head,QString verein="");
    QStringList readDetailInfos(QString verein);
    void printCustomPage(int mode, int layoutid, QStringList tndata=QStringList(), QString tnwk="");
    void drawHighlightRect(qreal y, qreal h=-1);
    void drawTextLine(QString text, int x=0, bool newLine=true);


    int outputType;
    bool showPreview;
    bool selectClub;
    bool selectRiege;
    bool selectTN;
    bool selectWK;
    bool selectDis;
    bool selectDetail;
    bool finish;
    static int headFootID;
    static int coverID;
    QString outputFileName;
    static QPrinter::PaperSize paperSize;
    static QPrinter::Orientation orientation;

    QList<int> vereinNumbers;
    QList< QList<int> > disziplinenIDs;
    QString detailQuery;
    QString teilnehmerString;
    QStringList wkNumbers;
    QString singleWK;
    QList<bool> wkWahl;
    QList<bool> wkKP;
    QStringList riegenNumbers;
    QMap<QString,QImage> customImages;


    QList<int> selectedClubs;
    QStringList selectedWKs;
    QStringList selectedRiegen;
    QList<int> selectedTN;
    QString selectedTNWK;
    QList<int> teilnehmerNumbers;
    QList< QList<int> > selectedDisziplinen;

signals:
    void requestDetailInfo();
    void requestDisziplinen();
    void requestWKs();
    void requestTN();
    void requestVereine();
    void requestRiegen();
    void showPrintPreview(QPrinter*);
};

#endif // DRUCKEN_H
