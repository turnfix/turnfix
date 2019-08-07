#ifndef DISCIPLINEGROUPITEMMODEL_H
#define DISCIPLINEGROUPITEMMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Discipline;
class DisciplineGroup;
class DisciplineGroupItem;

class DisciplineGroupItemModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DisciplineGroupItemModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchItems(DisciplineGroup *group);
    void addItem(Discipline *discipline);
    void removeItem(const QModelIndex &index);

    void moveUp(const QModelIndex &index);
    void moveDown(const QModelIndex &index);

    void persistChanges();

private:
    QList<DisciplineGroupItem *> m_items;
    QList<DisciplineGroupItem *> m_removedItems;
    DisciplineGroup *m_disciplineGroup;
    EntityManager *m_em;
};

#endif // DISCIPLINEGROUPITEMMODEL_H
