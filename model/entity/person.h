#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class DBTable;

class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
    Q_PROPERTY(QString address READ address WRITE setAddress)
    Q_PROPERTY(QString zip READ zip WRITE setZip)
    Q_PROPERTY(QString city READ city WRITE setCity)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber)
    Q_PROPERTY(QString faxNumber READ faxNumber WRITE setFaxNumber)
    Q_PROPERTY(QString email READ email WRITE setEmail)

public:
    Q_INVOKABLE Person() = default;

    int id() const;
    void setId(int id);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString address() const;
    void setAddress(const QString &address);

    QString fullName() const;

    QString zip() const;
    void setZip(const QString &zip);

    QString city() const;
    void setCity(const QString &city);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    QString faxNumber() const;
    void setFaxNumber(const QString &faxNumber);

    QString email() const;
    void setEmail(const QString &email);

    static const DBTable *mapping();

private:
    int m_id = 0;
    QString m_firstName;
    QString m_lastName;
    QString m_address;
    QString m_zip;
    QString m_city;
    QString m_phoneNumber;
    QString m_faxNumber;
    QString m_email;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Person *);

#endif // PERSON_H
