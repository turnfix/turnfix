#ifndef DBINITIALIZERPOSTGRES_H
#define DBINITIALIZERPOSTGRES_H

#include <QThread>

class DBInitializerPostgres : public QThread
{
    Q_OBJECT
public:
    explicit DBInitializerPostgres(QObject *parent = 0);
    void setData(QString adminUser, QString adminPass, QString tfUser,
                 QString tfPass, QString databaseName, QString server);

protected:
    void run();

private:
    QString adminUser;
    QString adminPass;
    QString tfUser;
    QString tfPass;
    QString databaseName;
    QString server;

signals:
    void statusUpdate(int stage, bool successful,QString message="");
};

#endif // DBINITIALIZERPOSTGRES_H
