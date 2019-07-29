#ifndef DBCONSTRAINT_H
#define DBCONSTRAINT_H

#include <QObject>

class DBColumn;

class DBConstraint : public QObject
{
    Q_OBJECT
public:
    explicit DBConstraint(QString name,
                          QString referenceTable,
                          QString fromField,
                          QString toField,
                          QString onUpdate,
                          QString onDelete,
                          DBColumn *parent = nullptr);

    QString name();
    QString referenceTable();
    QString fromField();
    QString toField();
    QString onUpdate();
    QString onDelete();

    void check(DBConstraint *compareConstraint, const QString &connectionName);

private:
    void create(const QString &connectionName);

    DBColumn *column();

    QString m_name;
    QString m_referenceTable;
    QString m_fromField;
    QString m_toField;
    QString m_onUpdate;
    QString m_onDelete;
};

#endif // DBCONSTRAINT_H
