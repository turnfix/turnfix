#include "sqliteconnection.h"

QString SQLiteConnection::getFileName() const
{
    return fileName;
}

void SQLiteConnection::setFileName(const QString &value)
{
    fileName = value;
}
