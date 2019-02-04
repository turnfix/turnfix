#ifndef TRD_CHECKTABLESTHREAD_H
#define TRD_CHECKTABLESTHREAD_H

#include <QThread>
#include <QVariant>

class CheckTable_Thread : public QThread {
    Q_OBJECT

public:
    bool getCheck();

protected:
    void run();

private:
    struct column {
        QString name;
        QString typ;
        int length;
        QString nullable;
        QVariant def;
        QString use;
    };

    struct cons {
        QString name;
        QString typ;
        QString column;
        QString ftable;
        QString fcolumn;
        QString onUpdate;
        QString onDelete;
    };
    bool check;
    QString varchar(int length, bool showLenght=true);
    QString primary();

signals:
    void newStatus(const QString&, const QString&);

};

#endif // TRD_CHECKTABLESTHREAD_H
