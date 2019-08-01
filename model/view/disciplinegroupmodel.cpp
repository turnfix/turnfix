#include "disciplinegroupmodel.h"
#include "model/entity/disciplinegroup.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/disciplinegrouprepository.h"

DisciplineGroupModel::DisciplineGroupModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineGroupModel::rowCount(const QModelIndex &) const
{
    return m_groups.size();
}

int DisciplineGroupModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DisciplineGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Kommentar");
        }
    }
    return QVariant();
}

QVariant DisciplineGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DisciplineGroup *group = m_groups.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return group->name();
        case 1:
            return group->comment();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(group);
    } else if (role == TF::IdRole) {
        return group->id();
    }
    return QVariant();
}

void DisciplineGroupModel::fetchGroups()
{
    beginResetModel();
    m_groups = m_em->disciplineGroupRepository()->loadAll();
    endResetModel();
}
