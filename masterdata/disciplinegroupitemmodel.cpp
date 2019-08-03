#include "disciplinegroupitemmodel.h"
#include "model/entity/disciplinegroupitem.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/disciplinegroupitemrepository.h"
#include "model/repository/disciplinerepository.h"

DisciplineGroupItemModel::DisciplineGroupItemModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineGroupItemModel::rowCount(const QModelIndex &) const
{
    return m_disciplines.size();
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

    auto item = m_disciplines.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return item->name();
        case 1:
            return item->sport()->name();
        }
    } else if (role == Qt::CheckStateRole) {
        switch (index.column()) {
        case 0:
            return m_items.contains(item->id()) ? Qt::Checked : Qt::Unchecked;
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(item);
    } else if (role == TF::IdRole) {
        return item->id();
    }
    return QVariant();
}

bool DisciplineGroupItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        auto discipline = m_disciplines.at(index.row());
        if (index.column() == 0 && role == Qt::CheckStateRole) {
            if (value == Qt::Checked) {
                auto item = new DisciplineGroupItem();
                item->setDiscipline(discipline);
                item->setDisciplineGroup(m_disciplineGroup);
                item->setSort(index.row());
                m_items.insert(discipline->id(), item);
            } else {
                m_removedItems.append(m_items.take(discipline->id()));
            }
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DisciplineGroupItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == 0) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEnabled;
}

void DisciplineGroupItemModel::fetchItems(DisciplineGroup *group)
{
    m_disciplineGroup = group;

    beginResetModel();
    m_disciplines = m_em->disciplineRepository()->loadAll();
    auto items = m_em->disciplineGroupItemRepository()->loadByGroup(group);
    foreach (auto item, items) {
        m_items.insert(item->disciplineId(), item);
        qDebug() << item->discipline() << m_disciplines.indexOf(item->discipline());
        m_disciplines.move(m_disciplines.indexOf(item->discipline()), item->sort());
    }
    endResetModel();
}

void DisciplineGroupItemModel::moveUp(const QModelIndex &index)
{
    int row = index.row();

    if (row <= 0) {
        return;
    }

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
    m_disciplines.swapItemsAt(row, row - 1);
    auto id = m_disciplines.at(row)->id();
    if (m_items.contains(id)) {
        auto discipline = m_items.value(id);
        discipline->setSort(row - 1);
        m_items.insert(id, discipline);
    }

    auto id2 = m_disciplines.at(row - 1)->id();
    if (m_items.contains(id2)) {
        auto discipline = m_items.value(id2);
        discipline->setSort(row);
        m_items.insert(id2, discipline);
    }
    endMoveRows();
}

void DisciplineGroupItemModel::moveDown(const QModelIndex &index)
{
    int row = index.row();

    if (row >= rowCount()) {
        return;
    }

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
    m_disciplines.swapItemsAt(row, row + 1);
    auto id = m_disciplines.at(row)->id();
    if (m_items.contains(id)) {
        auto discipline = m_items.value(id);
        discipline->setSort(row + 1);
        m_items.insert(id, discipline);
    }

    auto id2 = m_disciplines.at(row + 1)->id();
    if (m_items.contains(id2)) {
        auto discipline = m_items.value(id2);
        discipline->setSort(row);
        m_items.insert(id2, discipline);
    }

    endMoveRows();
}

void DisciplineGroupItemModel::persistChanges()
{
    QList<int> keys = m_items.keys();
    for (int i = 0; i < keys.length(); i++) {
        auto item = m_items.value(keys.at(i));
        m_em->disciplineGroupItemRepository()->persist(item);
    }
    foreach (DisciplineGroupItem *item, m_removedItems) {
        m_em->disciplineGroupItemRepository()->remove(item);
    }
}
