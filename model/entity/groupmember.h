#ifndef GROUPMEMBER_H
#define GROUPMEMBER_H

#include <QObject>

#include "athlete.h"
#include "group.h"

class DBTable;

class GroupMember : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int athleteId READ athleteId WRITE setAthleteId)
    Q_PROPERTY(int groupId READ groupId WRITE setGroupId)

    Q_PROPERTY(Athlete *athlete READ athlete WRITE setAthlete STORED false)
    Q_PROPERTY(Group *group READ group WRITE setGroup STORED false)

public:
    Q_INVOKABLE GroupMember() = default;

    int id() const;
    void setId(int id);

    int athleteId() const;
    void setAthleteId(int athleteId);

    Athlete *athlete() const;
    void setAthlete(Athlete *value);

    int groupId() const;
    void setGroupeId(int groupId);

    Group *group() const;
    void setGroup(Group *value);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_athleteId;
    Athlete *m_athlete;

    int m_groupeId;
    Group *m_group;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(GroupMember *);

#endif // GROUPMEMBER_H
