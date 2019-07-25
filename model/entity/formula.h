#ifndef FORMULA_H
#define FORMULA_H

#include <QObject>

class DBTable;

class Formula : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString formula READ formula WRITE setFormula)
    Q_PROPERTY(int type READ type WRITE setType)

public:
    Q_INVOKABLE Formula() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString formula() const;
    void setFormula(const QString &formula);

    int type() const;
    void setType(int type);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    QString m_formula;
    int m_type;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Formula *);

#endif // FORMULA_H
