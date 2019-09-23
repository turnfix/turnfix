#ifndef DBCHECKER_H
#define DBCHECKER_H

#include <QThread>

class AbstractConnection;
class DBTable;

class DBChecker : public QThread
{
    Q_OBJECT
public:
    explicit DBChecker(AbstractConnection *connection, QObject *parent = nullptr);

protected:
    void run();

private slots:
    void columnUpdate(int);

private:
    AbstractConnection *connection;
    QList<const DBTable *> tableData();

signals:
    void tableCount(int);
    void currentTable(QString);
    void tablesChecked(int);
    void columnFKCount(int);
    void columnsChecked(int);

};

#endif // DBCHECKER_H
