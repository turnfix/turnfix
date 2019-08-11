#ifndef DISCIPLINEPOSITION_H
#define DISCIPLINEPOSITION_H

#include <QObject>

class DBTable;
class CompetitionDiscipline;

class DisciplinePosition : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE DisciplinePosition() = default;

    int id() const;
    void setId(int id);

    int competitionDisciplineId() const;
    void setCompetitionDisciplineId(int competitionDisciplineId);

    CompetitionDiscipline *competitionDiscipline() const;
    void setCompetitionDiscipline(CompetitionDiscipline *competitionDiscipline);

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    int type() const;
    void setType(int type);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_competitionDisciplineId;
    CompetitionDiscipline *m_competitionDiscipline;
    int m_x;
    int m_y;
    int m_type;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(DisciplinePosition *);

#endif // DISCIPLINEPOSITION_H
