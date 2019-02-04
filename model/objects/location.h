#ifndef LOCATION_H
#define LOCATION_H

#include "tfobject.h"

class Location : public TFObject
{
    Q_OBJECT
public:
    QString getName();
    QString getCity();

    QString getNameAndCity();

    static QMap<int,Location*> getLocationsWithIds(QList<int> ids);

private:
    int id;
    QString name;
    QString address;
    QString zip;
    QString city;

    void setData(TFSqlQuery query);

};

#endif // LOCATION_H
