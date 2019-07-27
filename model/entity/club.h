#ifndef CLUB_H
#define CLUB_H

#include "person.h"
#include <QObject>

class DBTable;

class Club : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int regionId READ regionId WRITE setRegionId)
    Q_PROPERTY(int contactPersonId READ contactPersonId WRITE setContactPersonId)
    Q_PROPERTY(int posCity READ posCity WRITE setPosCity)
    Q_PROPERTY(QString website READ website WRITE setWebsite)

    Q_PROPERTY(Person *contactPerson READ contactPerson WRITE setContactPerson STORED false)

public:
    Q_INVOKABLE Club() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    int regionId() const;
    void setRegionId(int regionId);

    int contactPersonId() const;
    void setContactPersonId(int contactPersonId);

    Person *contactPerson() const;
    void setContactPerson(Person *contactPerson);

    int posCity() const;
    void setPosCity(int posCity);

    QString website() const;
    void setWebsite(const QString &website);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    int m_regionId;
    int m_contactPersonId;
    Person *m_contactPerson;
    int m_posCity;
    QString m_website;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Club *);

#endif // CLUB_H
