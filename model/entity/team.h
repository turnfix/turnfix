#ifndef TEAM_H
#define TEAM_H

#include "club.h"
#include "competition.h"
#include <QObject>

class DBTable;

class Team : public QObject
{
public:
    int id() const;
    void setId(int id);

    int clubId() const;
    void setClubId(int clubId);

    Club *club() const;
    void setClub(Club *value);

    int competitionId() const;
    void setCompetitionId(int competitionId);

    Competition *competition() const;
    void setCompoetition(Competition *value);

    int number() const;
    void setNumber(int number);

    QString squad() const;
    void setSquad(const QString &squad);

    int startNumber() const;
    void setStartNumber(int startNumber);

    static const DBTable *mapping();
private:
    int m_id = 0;
    int m_clubId;
    Club *m_club;
    Competition *m_competition;
    int m_competitionId;
    int m_number;
    QString m_squad;
    int m_startNumber;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};

#endif // TEAM_H
