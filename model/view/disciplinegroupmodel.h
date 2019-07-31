#ifndef DISCIPLINEGROUPMODEL_H
#define DISCIPLINEGROUPMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class DisciplineGroup;

class DisciplineGroupModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DisciplineGroupModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchGroups();

private:
    QList<DisciplineGroup *> m_groups;
    EntityManager *m_em;
};

#endif // DISCIPLINEGROUPMODEL_H
