#include "disciplinegroupitemmodel.h"
#include "model/entity/disciplinegroupitem.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/disciplinegroupitemrepository.h"

DisciplineGroupItemModel::DisciplineGroupItemModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineGroupItemModel::rowCount(const QModelIndex &) const
{
    return m_items.size();
}

int DisciplineGroupItemModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DisciplineGroupItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Sport");
        }
    }
    return QVariant();
}

QVariant DisciplineGroupItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DisciplineGroupItem *item = m_items.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return item->discipline()->name();
        case 1:
            return item->discipline()->sport()->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(item);
    } else if (role == TF::IdRole) {
        return item->id();
    }
    return QVariant();
}

void DisciplineGroupItemModel::fetchItems(DisciplineGroup *group)
{
    m_disciplineGroup = group;
    qDebug() << group;
    beginResetModel();
    m_items = m_em->disciplineGroupItemRepository()->loadByGroup(group);
    endResetModel();
}

void DisciplineGroupItemModel::addItem(Discipline *discipline)
{
    qDebug() << m_disciplineGroup;
    auto item = new DisciplineGroupItem();
    item->setDiscipline(discipline);
    item->setDisciplineGroup(m_disciplineGroup);
    int position = m_items.size();
    beginInsertRows(QModelIndex(), position, position);
    m_items.append(item);
    endInsertRows();
}

void DisciplineGroupItemModel::removeItem(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_removedItems.append(m_items.takeAt(index.row()));
    endRemoveRows();
}

void DisciplineGroupItemModel::moveUp(const QModelIndex &index)
{
    if (index.row() <= 0) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() - 1);
    m_items.swapItemsAt(index.row(), index.row() - 1);
    endMoveRows();
}

void DisciplineGroupItemModel::moveDown(const QModelIndex &index)
{
    if (index.row() >= rowCount()) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() + 2);
    m_items.swapItemsAt(index.row(), index.row() + 1);
    endMoveRows();
}

void DisciplineGroupItemModel::persistChanges()
{
    for (int i = 0; i < m_items.length(); i++) {
        auto item = m_items.at(i);
        item->setSort(i);
        m_em->disciplineGroupItemRepository()->persist(item);
    }
    foreach (DisciplineGroupItem *item, m_removedItems) {
        m_em->disciplineGroupItemRepository()->remove(item);
    }
}
