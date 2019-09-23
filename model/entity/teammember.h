#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>

#include "athlete.h"
#include "team.h"

class DBTable;

class TeamMember : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int teamId READ teamId WRITE setTeamId)
    Q_PROPERTY(int athleteId READ athleteId WRITE setAthleteId)
    Q_PROPERTY(int round READ round WRITE setRound)

    Q_PROPERTY(Team *team READ team WRITE setTeam STORED false)
    Q_PROPERTY(Athlete *athlete READ athlete WRITE setAthlete STORED false)

public:
    Q_INVOKABLE TeamMember() = default;

    int id() const;
    void setId(int id);

    int teamId() const;
    void setTeamId(int teamId);

    Team *team() const;
    void setTeam(Team *team);

    int athleteId() const;
    void setAthleteId(int athleteId);

    Athlete *athlete() const;
    void setAthlete(Athlete *athlete);

    int round() const;
    void setRound(int round);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_teamId;
    Team *m_team;
    int m_athleteId;
    Athlete *m_athlete;
    int m_round;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(TeamMember *);

#endif // TEAMMEMBER_H
