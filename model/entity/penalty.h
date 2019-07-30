#ifndef PENALTY_H
#define PENALTY_H

#include <QObject>

class DBTable;

class Penalty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(double deduction READ deduction WRITE setDeduction)

public:
    Q_INVOKABLE Penalty() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    double deduction() const;
    void setDeduction(double deduction);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    double m_deduction;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Penalty *);

#endif // PENALTY_H
