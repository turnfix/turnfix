#ifndef TFOBJECT_H
#define TFOBJECT_H

#include <QObject>
#include "model/database/tfsqlquery.h"

class TFObject : public QObject
{
    Q_OBJECT
public:
    explicit TFObject(QObject *parent = 0);

private:
    virtual void setData(TFSqlQuery query)=0;

};

#endif // TFOBJECT_H
