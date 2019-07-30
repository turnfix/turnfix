#include "venuemodel.h"
#include "model/entity/venue.h"
#include "model/entitymanager.h"
#include "model/repository/venuerepository.h"

VenueModel::VenueModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
    
}

int VenueModel::rowCount(const QModelIndex &) const
{
    return m_venues.size();
}

int VenueModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant VenueModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("PLZ");
        case 2:
            return tr("Ort");
        }
    }
    return QVariant();
}

QVariant VenueModel::data(const QModelIndex &index, int role) const
{
    Venue *venue = m_venues.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return venue->name();
        case 1:
            return venue->zip();
        case 2:
            return venue->city();
        case 3:
            return QString("%1, %2").arg(venue->name(), venue->city());
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(venue);
    }
    return QVariant();
}

void VenueModel::fetchVenues()
{
    beginResetModel();
    m_venues = m_em->venueRepository()->loadAll();
    endResetModel();
}
