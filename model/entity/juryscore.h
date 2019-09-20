#ifndef JURYSCORE_H
#define JURYSCORE_H

#include <QObject>

#include "disciplinefield.h"
#include "score.h"

class DBTable;

class JuryScore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int scoreId READ scoreId WRITE setScoreId)
    Q_PROPERTY(int disciplineFieldId READ disciplineFieldId WRITE setDisciplineFieldId)
    Q_PROPERTY(int round READ round WRITE setRound)
    Q_PROPERTY(int attempt READ attempt WRITE setAttempt)
    Q_PROPERTY(int type READ type WRITE setType)
    Q_PROPERTY(double performance READ performance WRITE setPerformance)

    Q_PROPERTY(Score *score READ score WRITE setScore STORED false)
    Q_PROPERTY(
        DisciplineField *disciplineField READ disciplineField WRITE setDisciplineField STORED false)

public:
    Q_INVOKABLE JuryScore() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int scoreId() const;
    void setScoreId(int scoreId);

    Score *score() const;
    void setScore(Score *score);

    int disciplineFieldId() const;
    void setDisciplineFieldId(int disciplineFieldId);

    DisciplineField *disciplineField() const;
    void setDisciplineField(DisciplineField *disciplineField);

    int round() const;
    void setRound(int round);

    int attempt() const;
    void setAttempt(int count);

    double performance() const;
    void setPerformance(double performance);

    int type() const;
    void setType(int type);

private:
    int m_id = 0;
    int m_scoreId;
    Score *m_score;
    int m_disciplineFieldId;
    DisciplineField *m_disciplineField;
    int m_round;
    int m_attempt;
    double m_performance;
    int m_type;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(JuryScore *);

#endif // JURYSCORE_H
