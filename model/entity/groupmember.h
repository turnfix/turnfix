#ifndef GROUPMEMBER_H
#define GROUPMEMBER_H

#include <QObject>
#include "athlete.h"
#include "groupe.h"

class DBTable;

class GroupeMember : public QObject
{
public:
    int id() const;
    void setId(int id);

    int athleteId() const;
    void setAthleteId(int athleteId);

    Athlete *athlete() const;
    void setAthlete(Athlete *value);

    int groupeId() const;
    void setGroupeId(int groupeId);

    Groupe *groupe() const;
    void setGroupe(Groupe *value);

    static const DBTable *mapping();
private:
    int m_id = 0;
    int m_athleteId;
    Athlete *m_athlete;

    int m_groupeId;
    Groupe *m_groupe;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};

#endif // GROUPMEMBER_H
