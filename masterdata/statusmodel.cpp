#include "statusmodel.h"
#include "model/entity/status.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/statusrepository.h"

StatusModel::StatusModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int StatusModel::rowCount(const QModelIndex &) const
{
    return m_statuses.size();
}

int StatusModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant StatusModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        }
    }
    return QVariant();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Status *status = m_statuses.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return status->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(status);
    } else if (role == TF::IdRole) {
        return status->id();
    }
    return QVariant();
}

void StatusModel::fetchStatuses()
{
    beginResetModel();
    m_statuses = m_em->statusRepository()->loadAll();
    endResetModel();
}
