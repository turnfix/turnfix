#ifndef SCOREDISCIPLINE_H
#define SCOREDISCIPLINE_H

#include <QObject>

#include "discipline.h"
#include "score.h"

class DBTable;

class ScoreDiscipline : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int scoreId READ scoreId WRITE setScoreId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)

    Q_PROPERTY(Score *score READ score WRITE setScore STORED false)
    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)

public:
    Q_INVOKABLE ScoreDiscipline() = default;

    int id() const;
    void setId(int id);

    int scoreId() const;
    void setScoreId(int scoreId);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Score *score() const;
    void setScore(Score *score);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_scoreId;
    Score *m_score;
    int m_disciplineId;
    Discipline *m_discipline;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(ScoreDiscipline *);

#endif // SCOREDISCIPLINE_H
