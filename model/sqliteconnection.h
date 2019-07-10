#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "abstractconnection.h"

class SQLiteConnection : public AbstractConnection
{
public:
    QString getFileName() const;
    void setFileName(const QString &value);

private:
    QString fileName;
};

#endif // SQLITECONNECTION_H
