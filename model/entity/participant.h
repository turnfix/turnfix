#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QObject>
#include <QDate>
#include "model/entity/club.h"

class DBTable;

class Participant : public QObject
{
public:
    Q_INVOKABLE Participant() = default;
    int id() const;
    void setId(int id);

    int clubId() const;
    void setClubId(int clubId);

    static const DBTable *mapping();
    Club *club();
    void setClub(Club *club);

    void setFirstName(const QString &fistName);
    QString firstName() const;

    void setLastName(const QString &lastName);
    QString lastName() const;

    void setGender(const int &gender);
    int gender() const;

    void setBirthday(const QDate &birthday);
    QDate birthday() const;

    void setOnlyYear(const bool &onlyYear);
    bool onlyYear() const;

    void setStartPass(const int &startPass);
    int startPass() const;

private:
    int m_id = 0;
    int m_clubId;
    Club *m_club;

    QString m_firstName;
    QString m_lastName;

    int m_gender;
    QDate m_birthday;
    bool m_onlyYear;
    int m_startPass;


    static DBTable *initializeMapping();
    static const DBTable *m_mapping;

};
Q_DECLARE_METATYPE(Participant *);

#endif // PARTICIPANT_H
