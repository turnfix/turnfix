#include "clubmodel.h"
#include "model/entity/club.h"
#include "model/entitymanager.h"
#include "model/repository/clubrepository.h"

ClubModel::ClubModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int ClubModel::rowCount(const QModelIndex &) const
{
    return m_clubs.size();
}

int ClubModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant ClubModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Ansprechpartner");
        case 2:
            return tr("Website");
        }
    }
    return QVariant();
}

QVariant ClubModel::data(const QModelIndex &index, int role) const
{
    Club *club = m_clubs.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return club->name();
        case 1:
            return club->contactPerson()->fullName();
        case 2:
            return club->website();
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(club);
    }
    return QVariant();
}

void ClubModel::fetchClubs()
{
    beginResetModel();
    m_clubs = m_em->clubRepository()->loadAll();
    endResetModel();
}
