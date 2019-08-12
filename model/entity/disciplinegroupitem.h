#ifndef DISCIPLINEGROUPITEM_H
#define DISCIPLINEGROUPITEM_H

#include "discipline.h"
#include "disciplinegroup.h"
#include <QObject>

class DBTable;

class DisciplineGroupItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int disciplineGroupId READ disciplineGroupId WRITE setDisciplineGroupId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(int sort READ sort WRITE setSort)

    Q_PROPERTY(Discipline *discipline READ discipline WRITE setDiscipline STORED false)
    Q_PROPERTY(
        DisciplineGroup *disciplineGroup READ disciplineGroup WRITE setDisciplineGroup STORED false)

public:
    Q_INVOKABLE DisciplineGroupItem() = default;

    static const DBTable *mapping();

    int id() const;
    void setId(int id);

    int disciplineGroupId() const;
    void setDisciplineGroupId(int disciplineGroupId);

    DisciplineGroup *disciplineGroup() const;
    void setDisciplineGroup(DisciplineGroup *disciplineGroup);

    int disciplineId() const;
    void setDisciplineId(int disciplineId);

    Discipline *discipline() const;
    void setDiscipline(Discipline *discipline);

    int sort() const;
    void setSort(int sort);

private:
    int m_id = 0;
    int m_disciplineGroupId;
    DisciplineGroup *m_disciplineGroup;
    int m_disciplineId;
    Discipline *m_discipline;
    int m_sort;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(DisciplineGroupItem *);

#endif // DISCIPLINEGROUPITEM_H
