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

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void fetchItems(DisciplineGroup *group);

    void moveUp(const QModelIndex &index);
    void moveDown(const QModelIndex &index);

    void persistChanges();

private:
    QMap<int, DisciplineGroupItem *> m_items;
    QList<DisciplineGroupItem *> m_removedItems;
    QList<Discipline *> m_disciplines;
    DisciplineGroup *m_disciplineGroup;
    EntityManager *m_em;
};

#endif // DISCIPLINEGROUPITEMMODEL_H
