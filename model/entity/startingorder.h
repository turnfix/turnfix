#ifndef STARTINGORDER_H
#define STARTINGORDER_H

#include <QObject>

#include "discipline.h"
#include "score.h"

class DBTable;

class StartingOrder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int scoreId READ scoreId WRITE setScoreId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(int position READ position WRITE setPosition)
    Q_PROPERTY(int type READ type WRITE setType)

    Q_PROPERTY(Score *score READ score WRITE setScore STORED false)
    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)

public:
    Q_INVOKABLE StartingOrder() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int scoreId() const;
    void setScoreId(int scoreId);

    Score *score() const;
    void setScore(Score *score);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    int position() const;
    void setPosition(int position);

    int type() const;
    void setType(int type);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

private:
    int m_id = 0;
    int m_scoreId;
    Score *m_score;
    int m_disciplineId;
    Discipline *m_discipline;
    int m_position;
    int m_type;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(StartingOrder *);

#endif // STARTINGORDER_H
