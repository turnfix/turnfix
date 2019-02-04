#ifndef DBCOLUMN_H
#define DBCOLUMN_H

#include <QObject>

class DBColumn : public QObject
{
    Q_OBJECT
public:
    explicit DBColumn(QString name, QString type, int length=0,
                      bool null=true, QString defaultValue="", QString extraQuery="", bool pk=false, QObject *parent = 0);
    QString getName();
    QString getType();
    QString getTypeWithLength();
    int getLength();
    bool getNull();
    QString getNullString();
    QString getDefaultValue();
    QString getExtraQuery();
    bool getPK();

private:
    QString name;
    QString type;
    int length;
    bool null;
    QString defaultValue;
    QString extraQuery;
    bool pk;

};

#endif // DBCOLUMN_H
