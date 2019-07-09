#ifndef DBCONSTRAINT_H
#define DBCONSTRAINT_H

#include <QObject>

class DBConstraint : public QObject
{
    Q_OBJECT
public:
    explicit DBConstraint(QString name, QString referenceTable, QString fromField,
                          QString toField, QString onUpdate,
                          QString onDelete, QObject *parent = 0);

    QString getName();
    QString getReferenceTable();
    QString getFromField();
    QString getToField();
    QString getOnUpdate();
    QString getOnDelete();

private:
    QString name;
    QString referenceTable;
    QString fromField;
    QString toField;
    QString onUpdate;
    QString onDelete;

};

#endif // DBCONSTRAINT_H
