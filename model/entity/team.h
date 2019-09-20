#ifndef TEAM_H
#define TEAM_H

#include "club.h"
#include "competition.h"
#include <QObject>

class DBTable;

class Team : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int clubId READ clubId WRITE setClubId)
    Q_PROPERTY(int competitionId READ competitionId WRITE setCompetitionId)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString squad READ squad WRITE setSquad)
    Q_PROPERTY(int bib READ bib WRITE setBib)

    Q_PROPERTY(Club *club READ club WRITE setClub STORED false)
    Q_PROPERTY(Competition *competition READ competition WRITE setCompetition STORED false)

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

    int bib() const;
    void setBib(int bib);

    static const DBTable *mapping();
private:
    int m_id = 0;
    int m_clubId;
    Club *m_club;
    Competition *m_competition;
    int m_competitionId;
    int m_number;
    QString m_squad;
    int m_bib;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};

#endif // TEAM_H
