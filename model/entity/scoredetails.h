#ifndef SCOREDETAILS_H
#define SCOREDETAILS_H

#include <QObject>

#include "discipline.h"
#include "score.h"

class DBTable;

class ScoreDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int scoreId READ scoreId WRITE setScoreId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(int attempt READ attempt WRITE setAttempt)
    Q_PROPERTY(double performance READ performance WRITE setPerformance)
    Q_PROPERTY(int type READ type WRITE setType)

public:
    Q_INVOKABLE ScoreDetails() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int scoreId() const;
    void setScoreId(int scoreId);

    Score *score() const;
    void setScore(Score *score);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    int attempt() const;
    void setAttempt(int attempt);

    double performance() const;
    void setPerformance(double performance);

    int type() const;
    void setType(int type);

private:
    int m_id = 0;
    int m_scoreId;
    Score *m_score;
    int m_disciplineId;
    Discipline *m_discipline;
    int m_attempt;
    double m_performance;
    int m_type;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(ScoreDetails *);

#endif // SCOREDETAILS_H
