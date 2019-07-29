#include "sportmodel.h"
#include "model/entity/sport.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/sportrepository.h"

SportModel::SportModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{}

int SportModel::rowCount(const QModelIndex &) const
{
    return m_sports.size();
}

int SportModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant SportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        }
    }
    return QVariant();
}

QVariant SportModel::data(const QModelIndex &index, int role) const
{
    Sport *sport = m_sports.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return sport->name();
        }
    } else if (role == TF::ItemDataRole::ObjectRole) {
        return QVariant::fromValue(sport);
    } else if (role == TF::ItemDataRole::IdRole) {
        return sport->id();
    }
    return QVariant();
}

void SportModel::fetchSports()
{
    beginResetModel();
    m_sports = m_em->sportRepository()->loadAll();
    endResetModel();
}
