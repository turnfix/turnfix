#ifndef TEAMPENALTY_H
#define TEAMPENALTY_H

#include <QObject>

#include "penalty.h"
#include "team.h"

class DBTable;

class TeamPenalty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int teamId READ teamId WRITE setTeamId)
    Q_PROPERTY(int penaltyId READ penaltyId WRITE setPenaltyId)

    Q_PROPERTY(Team *team READ team WRITE setTeam STORED false)
    Q_PROPERTY(Penalty *penalty READ penalty WRITE setPenalty STORED false)

public:
    Q_INVOKABLE TeamPenalty() = default;

    int id() const;
    void setId(int id);

    int teamId() const;
    void setTeamId(int teamId);

    int penaltyId() const;
    void setPenaltyId(int penaltyId);

    static const DBTable *mapping();

    Team *team() const;
    void setTeam(Team *team);

    Penalty *penalty() const;
    void setPenalty(Penalty *penalty);

private:
    int m_id = 0;
    int m_teamId;
    Team *m_team;
    int m_penaltyId;
    Penalty *m_penalty;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(TeamPenalty *);

#endif // TEAMPENALTY_H
