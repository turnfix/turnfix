#include <QDebug>
#include "dbchecker.h"
#include "dbtable.h"

DBChecker::DBChecker(QObject *parent) : QThread(parent) {}

void DBChecker::run()
{
    DBTable *table;
    QList<DBTable*> tables;

    DBTable::setupTables();
    tables = DBTable::getTables();

    emit tableCount(tables.size());

    for (int i=0;i<tables.size();i++)
    {
        table = tables.at(i);
        emit currentTable(tr("Überprüfe %1... %2 von %3").arg(table->getName()).arg(i+1).arg(tables.size()));
        emit columnFKCount(table->getColumnFKCount());
        connect(table,SIGNAL(columnsChecked(int)),this,SLOT(columnUpdate(int)));
        table->checkTable();
        emit tablesChecked(i+1);
    }

    emit currentTable("Datenbanküberprüfung/-aktualisierung erfolgreich abgeschlossen.");

    DBTable::clearTables();
}

void DBChecker::columnUpdate(int count)
{
    emit columnsChecked(count);
}
