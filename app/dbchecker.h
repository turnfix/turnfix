#ifndef DBCHECKER_H
#define DBCHECKER_H

#include <QThread>

class DBChecker : public QThread
{
    Q_OBJECT
public:
    explicit DBChecker(QObject *parent = 0);

protected:
    void run();

private slots:
    void columnUpdate(int);

signals:
    void tableCount(int);
    void currentTable(QString);
    void tablesChecked(int);
    void columnFKCount(int);
    void columnsChecked(int);

};

#endif // DBCHECKER_H
