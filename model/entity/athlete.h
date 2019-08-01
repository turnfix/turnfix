#ifndef ATHLETE_H
#define ATHLETE_H

#include "club.h"
#include <QDate>
#include <QObject>

class DBTable;

class Athlete : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int clubId READ clubId WRITE setClubId)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
    Q_PROPERTY(int gender READ gender WRITE setGender)
    Q_PROPERTY(QDate dateOfBirth READ dateOfBirth WRITE setDateOfBirth)
    Q_PROPERTY(bool yearOfBirthOnly READ yearOfBirthOnly WRITE setYearOfBirthOnly)
    Q_PROPERTY(QString license READ license WRITE setLicense)

    Q_PROPERTY(Club *club READ club WRITE setClub STORED false)

public:
    enum Gender { Female = 0, Male = 1 };

    Q_INVOKABLE Athlete() = default;

    int id() const;
    void setId(int id);

    int clubId() const;
    void setClubId(int clubId);

    Club *club() const;
    void setClub(Club *value);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString fullName() const;

    int gender() const;
    void setGender(const int &gender);

    QDate dateOfBirth() const;
    void setDateOfBirth(const QDate &dateOfBirth);

    bool yearOfBirthOnly() const;
    void setYearOfBirthOnly(bool yearOfBirthOnly);

    QString license() const;
    void setLicense(const QString &license);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_clubId;
    Club *m_club;
    QString m_firstName;
    QString m_lastName;
    int m_gender;
    QDate m_dateOfBirth;
    bool m_yearOfBirthOnly;
    QString m_license;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(Athlete *);

#endif // ATHLETE_H
