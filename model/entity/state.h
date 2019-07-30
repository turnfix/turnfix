#ifndef STATE_H
#define STATE_H

#include "country.h"
#include <QObject>

class DBTable;

class State : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int countryId READ countryId WRITE setCountryId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString code READ code WRITE setCode)

    Q_PROPERTY(Country *country READ country WRITE setCountry STORED false)

public:
    Q_INVOKABLE State() = default;

    int id() const;
    void setId(int id);

    int countryId() const;
    void setCountryId(int countryId);

    Country *country() const;
    void setCountry(Country *country);

    QString name() const;
    void setName(const QString &name);

    QString code() const;
    void setCode(const QString &code);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_countryId;
    Country *m_country;
    QString m_name;
    QString m_code;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(State *);

#endif // STATE_H
