#include "regionmodel.h"
#include "model/entity/region.h"
#include "model/entity/state.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/regionrepository.h"

RegionModel::RegionModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int RegionModel::rowCount(const QModelIndex &) const
{
    return m_regions.size();
}

int RegionModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant RegionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Kürzel");
        case 2:
            return tr("Verband");
        }
    }
    return QVariant();
}

QVariant RegionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Region *region = m_regions.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return region->name();
        case 1:
            return region->code();
        case 2:
            return region->state()->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(region);
    } else if (role == TF::IdRole) {
        return region->id();
    }
    return QVariant();
}

void RegionModel::fetchRegions()
{
    beginResetModel();
    m_regions = m_em->regionRepository()->loadAll();
    endResetModel();
}
