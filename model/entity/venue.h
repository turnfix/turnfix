#ifndef VENUE_H
#define VENUE_H

#include <QObject>

class DBTable;

class Venue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString address READ address WRITE setAddress)
    Q_PROPERTY(QString zip READ zip WRITE setZip)
    Q_PROPERTY(QString city READ city WRITE setCity)

public:
    Q_INVOKABLE Venue() = default;

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString address() const;
    void setAddress(const QString &address);

    QString zip() const;
    void setZip(const QString &zip);

    QString city() const;
    void setCity(const QString &city);

    QString nameAndCity();

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_name;
    QString m_address;
    QString m_zip;
    QString m_city;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Venue *);

#endif // VENUE_H
