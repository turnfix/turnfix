#ifndef WEB_UPLOAD_H
#define WEB_UPLOAD_H

#include <QSqlQuery>
#include <QDomDocument>
#include <QThread>
#include <QStringList>

class TOA_Upload : public QThread {
    Q_OBJECT;
public:
    void setRndRes(bool rndres);
    void setDetailInfo(int di);
    void setHeadFootID(int);
    void setAWExtra(bool);
    void setLandscape(bool);
protected:
    void run();
    void createPDF();
    QDomDocument createXML();
private:
    bool rndres;
    bool awExtra;
    bool landscape;
    int detailinfo;
    QStringList files;
    int headFoot;
    QDomElement createStarterElement(QString swknr="", int id=0, QDomDocument doc=QDomDocument("TurnFixResult"), QList<QStringList> rlist=QList<QStringList>());

signals:
    void textChanged(const QString&);
    void numberChanged(int,int);
    void uploadFiles(QStringList,QString);

};

#endif
