#include "sqliteconnection.h"
#include <QSqlError>
#include <QSqlQuery>

QString SQLiteConnection::fileName() const
{
    return m_fileName;
}

void SQLiteConnection::setFileName(const QString &value)
{
    m_fileName = value;
}

bool SQLiteConnection::connect(const QString &name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", name);
    db.setDatabaseName(m_fileName);

    bool successful = db.open();
    if (!successful) {
        emit errorOccured(db.lastError().text());
    } else {
        QSqlQuery("PRAGMA foreign_keys=true", db);
        m_databases.insert(name, db);
    }

    return successful;
}
