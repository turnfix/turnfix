#ifndef DISCIPLINEGROUPITEM_H
#define DISCIPLINEGROUPITEM_H

#include <QObject>

class DBTable;

class DisciplineGroupItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int disciplineGroupId READ disciplineGroupId WRITE setDisciplineGroupId)
    Q_PROPERTY(int disciplineId READ disciplineId WRITE setDisciplineId)
    Q_PROPERTY(int sort READ sort WRITE setSort)

public:
    Q_INVOKABLE DisciplineGroupItem() = default;

    static const DBTable *mapping();

private:
    int m_id = 0;
    int m_disciplineGroupId;
    int m_disciplineId;
    int m_sort;

    static DBTable *initializeMapping();
    static const DBTable *m_mapping;
};
Q_DECLARE_METATYPE(DisciplineGroupItem *);

#endif // DISCIPLINEGROUPITEM_H
