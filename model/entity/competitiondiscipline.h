#ifndef COMPETITIONDISCIPLINE_H
#define COMPETITIONDISCIPLINE_H

#include "competition.h"
#include "discipline.h"
#include <QObject>

class DBTable;

class CompetitionDiscipline : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int competitionId READ competitionId WRITE setCompetitionId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(QString invitationText READ invitationText WRITE setInvitationText)
    Q_PROPERTY(int sort READ sort WRITE setSort)
    Q_PROPERTY(bool freeAndCompulsary READ freeAndCompulsary WRITE setFreeAndCompulsary)
    Q_PROPERTY(double maximumScore READ maximumScore WRITE setMaximumScore)

    Q_PROPERTY(Competition *competition READ competition WRITE setCompetition STORED false)
    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)

public:
    Q_INVOKABLE CompetitionDiscipline() = default;

    int id() const;
    void setId(int id);

    int competitionId() const;
    void setCompetitionId(int competitionId);

    Competition *competition() const;
    void setCompetition(Competition *competition);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    QString invitationText() const;
    void setInvitationText(const QString &invitationText);

    int sort() const;
    void setSort(int sort);

    bool freeAndCompulsary() const;
    void setFreeAndCompulsary(bool freeAndCompulsary);

    double maximumScore() const;
    void setMaximumScore(double maximumScore);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_competitionId;
    Competition *m_competition;
    int m_disciplineId;
    Discipline *m_discipline;
    QString m_invitationText;
    int m_sort;
    bool m_freeAndCompulsary;
    double m_maximumScore;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;

signals:
    void freeAndCompulsaryChanged(bool freeAndCompulsary);
};
Q_DECLARE_METATYPE(CompetitionDiscipline *);

#endif // COMPETITIONDISCIPLINE_H
