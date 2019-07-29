#ifndef DISCIPLINEFIELD_H
#define DISCIPLINEFIELD_H

#include <QObject>

class DBTable;
class Discipline;

class DisciplineField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int sort READ sort WRITE setSort)
    Q_PROPERTY(bool finalScore READ finalScore WRITE setFinalScore)
    Q_PROPERTY(bool baseScore READ baseScore WRITE setBaseScore)
    Q_PROPERTY(int group READ group WRITE setGroup)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)

    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)

public:
    Q_INVOKABLE DisciplineField() = default;

    int id() const;
    void setId(int id);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    QString name() const;
    void setName(const QString &name);

    int sort() const;
    void setSort(int sort);

    bool finalScore() const;
    void setFinalScore(bool finalScore);

    bool baseScore() const;
    void setBaseScore(bool baseScore);

    int group() const;
    void setGroup(int group);

    bool enabled() const;
    void setEnabled(bool enabled);

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_disciplineId;
    Discipline *m_discipline;
    QString m_name;
    int m_sort;
    bool m_finalScore;
    bool m_baseScore;
    int m_group;
    bool m_enabled;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(DisciplineField *);

#endif // DISCIPLINEFIELD_H
