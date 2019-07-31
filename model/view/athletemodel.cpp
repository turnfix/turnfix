#include "athletemodel.h"
#include "model/entity/athlete.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/athleterepository.h"

AthleteModel::AthleteModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int AthleteModel::rowCount(const QModelIndex &) const
{
    return m_athletes.size();
}

int AthleteModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant AthleteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Geb.");
        case 2:
            return tr("Verein");
        }
    }
    return QVariant();
}

QVariant AthleteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Athlete *athlete = m_athletes.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return athlete->fullName();
        case 1:
            return athlete->dateOfBirth().toString();
        case 2:
            return athlete->club()->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(athlete);
    } else if (role == TF::IdRole) {
        return athlete->id();
    }
    return QVariant();
}

void AthleteModel::fetchAthletes()
{
    beginResetModel();
    m_athletes = m_em->athleteRepository()->loadAll();
    endResetModel();
}
