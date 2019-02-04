#ifndef TFSQLQUERY_H
#define TFSQLQUERY_H

#include <QSqlQuery>

class TFSettings;

class TFSqlQuery : public QSqlQuery
{

public:
    bool prepare(const QString &query);
    bool exec();
    bool exec(const QString &query);

    static QString intListToString(QList<int> list);

private:
    QString parseQuery(QString query);
};

#endif // TFSQLQUERY_H
