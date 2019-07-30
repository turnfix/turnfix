#ifndef COUNTRY_H
#define COUNTRY_H

#include <QObject>

class DBTable;

class Country : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString code READ code WRITE setCode)

public:
    Q_INVOKABLE Country() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString code() const;
    void setCode(const QString &code);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    QString m_code;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Country *);

#endif // COUNTRY_H
