#include "penaltymodel.h"
#include "model/entity/penalty.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/penaltyrepository.h"

PenaltyModel::PenaltyModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int PenaltyModel::rowCount(const QModelIndex &) const
{
    return m_penalties.size();
}

int PenaltyModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant PenaltyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Abzug");
        }
    }
    return QVariant();
}

QVariant PenaltyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Penalty *penalty = m_penalties.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return penalty->name();
        case 1:
            return penalty->deduction();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(penalty);
    } else if (role == TF::IdRole) {
        return penalty->id();
    }
    return QVariant();
}

void PenaltyModel::fetchPenalties()
{
    beginResetModel();
    m_penalties = m_em->penaltyRepository()->loadAll();
    endResetModel();
}
