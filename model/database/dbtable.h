#ifndef DBTABLE_H
#define DBTABLE_H

#include <QObject>

class DBColumn;
class DBConstraint;
class DB;

class DBTable : public QObject
{
    Q_OBJECT
public:
    explicit DBTable(QString name, QObject *parent = 0);
    void addColumn(DBColumn* column);
    void addContraint(DBConstraint* constraint);
    QString getName();
    int getColumnFKCount();

    void checkTable(DB* db);
    void create(DB* db);

    static void setupTables();
    static void clearTables();
    static QList<DBTable*> getTables();
    static int tableCount();

private:
    QString name;
    QList<DBColumn*> columns;
    QList<DBConstraint*> constraints;
    QString toTFType(QString type);
    static QList<DBTable*> tables;

signals:
    void tableOK();
    void tablesDoesNotExist();
    void columnsChecked(int);

};

#endif // DBTABLE_H
