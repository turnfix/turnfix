#ifndef JURYRESULTS_H
#define JURYRESULTS_H

#include <QObject>
#include "rating.h"
#include "disciplinefield.h"

class DBTable;

class Juryresults : public QObject
{

public:
    Q_INVOKABLE Juryresults() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    Rating *rating() const;
    void setRating(Rating *rating);
    int ratingId() const;

    DisciplineField *disciplineField() const;
    void setParticipant(DisciplineField *disciplineField);
    int disciplineId() const;

    int round() const;
    void setRound(int round);
    int trialCount() const;
    void settrialCount(int count);
    double performance() const;
    void setPerformance(double performance);

private:
    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
    int m_id = 0;

    int m_ratingId;
    int m_disciplineFieldId;
    Rating *m_rating;
    DisciplineField *m_disciplineField;

    int m_round;
    int m_trialCount;
    double m_performance;
};
#endif // JURYRESULTS_H
