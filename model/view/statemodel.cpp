#include "statemodel.h"
#include "model/entity/country.h"
#include "model/entity/state.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/staterepository.h"

StateModel::StateModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int StateModel::rowCount(const QModelIndex &) const
{
    return m_states.size();
}

int StateModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant StateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Kürzel");
        case 2:
            return tr("Land");
        }
    }
    return QVariant();
}

QVariant StateModel::data(const QModelIndex &index, int role) const
{
    State *state = m_states.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return state->name();
        case 1:
            return state->code();
        case 2:
            return state->country()->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(state);
    } else if (role == TF::IdRole) {
        return state->id();
    }
    return QVariant();
}

void StateModel::fetchStates()
{
    beginResetModel();
    m_states = m_em->stateRepository()->loadAll();
    endResetModel();
}
