#ifndef RATING_H
#define RATING_H

#include <QObject>
#include <model/entity/competition.h>
#include <model/entity/athlete.h>
#include <model/entity/groupe.h>
#include <model/entity/team.h>
#include <model/entity/state.h>

class DBTable;

class Rating : public QObject
{

public:
    Q_INVOKABLE Rating() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    Competition *competition() const;
    void setCompetition(Competition *competition);

    Athlete *participant() const;
    void setParticipant(Athlete *participant);

    Groupe *group() const;
    void setGroup(Groupe *group);

    Team *team() const;
    void setTeam(Team *team);

    State *state() const;
    void setState(State *state);

    int round() const;
    void setRound(int round);

    bool nonCompetitive() const;
    void setNonCompetitive(bool nonCompetitive);

    bool ill() const;
    void setIll(bool ill);

    QString squad() const;
    void setSquad(const QString &squad);

    QString comment() const;
    void setComment(const QString &comment);

private:
    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
    int m_id = 0;
    int m_competitionId;
    int m_participantId;
    int m_groupeId;
    int m_teamId;
    int m_stateId;
    int m_round;
    bool m_noncompetitive;
    bool m_ill;
    QString m_squad;
    QString m_comment;
};

#endif // RATING_H
