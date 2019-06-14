#ifndef DB_H
#define DB_H

#include <QObject>
#include "tfsqlquery.h"

class TFSettings;

class DB : public QObject
{
    Q_OBJECT
public:

    enum Typ { PostgreSQL, SQLite };

    bool establishConnection();
    void closeConnection();
    QSqlDatabase internal();

    int count(TFSqlQuery query);

    void emitError(QString);

    static DB* getInstance();
    static DB* createInstance(QString name);
    static void dropInstance();

public slots:

private:

    TFSettings *settings;
    static DB* instance;
    QString name;

    DB(QString name = "");
    DB(const DB &);
    DB& operator=(const DB &);

signals:
    void errorOccured(QString);

};

#endif // DB_H
