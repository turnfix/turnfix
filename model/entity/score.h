#ifndef SCORE_H
#define SCORE_H

#include <QObject>

#include "athlete.h"
#include "competition.h"
#include "group.h"
#include "status.h"
#include "team.h"

class DBTable;

class Score : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int competitionId READ competitionId WRITE setCompetitionId)
    Q_PROPERTY(int athleteId READ athleteId WRITE setAthleteId)
    Q_PROPERTY(int groupId READ groupId WRITE setGroupId)
    Q_PROPERTY(int teamId READ teamId WRITE setTeamId)
    Q_PROPERTY(int statusId READ statusId WRITE setStatusId)
    Q_PROPERTY(int round READ round WRITE setRound)
    Q_PROPERTY(bool nonCompetitive READ nonCompetitive WRITE setNonCompetitive)
    Q_PROPERTY(bool dns READ dns WRITE setDns)
    Q_PROPERTY(QString squad READ squad WRITE setSquad)
    Q_PROPERTY(QString comment READ comment WRITE setComment)

    Q_PROPERTY(Athlete *athlete READ athlete WRITE setAthlete STORED false)
    Q_PROPERTY(Group *group READ group WRITE setGroup STORED false)
    Q_PROPERTY(Team *team READ team WRITE setTeam STORED false)
    Q_PROPERTY(Status *status READ status WRITE setStatus STORED false)

public:
    Q_INVOKABLE Score() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    Competition *competition() const;
    void setCompetition(Competition *competition);

    int competitionId() const;
    void setCompetitionId(int competitionId);

    Athlete *athlete() const;
    void setAthlete(Athlete *athlete);

    int athleteId() const;
    void setAthleteId(int athleteId);

    Group *group() const;
    void setGroup(Group *group);

    int groupId() const;
    void setGroupId(int groupId);

    Team *team() const;
    void setTeam(Team *team);

    int teamId() const;
    void setTeamId(int teamId);

    Status *status() const;
    void setStatus(Status *status);

    int statusId() const;
    void setStatusId(int statusId);

    int round() const;
    void setRound(int round);

    bool nonCompetitive() const;
    void setNonCompetitive(bool nonCompetitive);

    bool dns() const;
    void setDns(bool dns);

    QString squad() const;
    void setSquad(const QString &squad);

    QString comment() const;
    void setComment(const QString &comment);

private:
    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
    int m_id = 0;
    int m_competitionId;
    Competition *m_competition;
    int m_athleteId;
    Athlete *m_athlete;
    int m_groupId;
    Group *m_group;
    int m_teamId;
    Team *m_team;
    int m_statusId;
    Status *m_status;
    int m_round;
    bool m_nonCompetitive;
    bool m_dns;
    QString m_squad;
    QString m_comment;
};
Q_DECLARE_METATYPE(Score *);

#endif // SCORE_H
