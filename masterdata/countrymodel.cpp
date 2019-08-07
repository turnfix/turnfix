#include "countrymodel.h"
#include "model/entity/country.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/countryrepository.h"

CountryModel::CountryModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int CountryModel::rowCount(const QModelIndex &) const
{
    return m_countries.size();
}

int CountryModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant CountryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Kürzel");
        }
    }
    return QVariant();
}

QVariant CountryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Country *country = m_countries.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return country->name();
        case 1:
            return country->code();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(country);
    } else if (role == TF::IdRole) {
        return country->id();
    }
    return QVariant();
}

void CountryModel::fetchCountries()
{
    beginResetModel();
    m_countries = m_em->countryRepository()->loadAll();
    endResetModel();
}
