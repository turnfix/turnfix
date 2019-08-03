#include "divisionmodel.h"
#include "model/entity/division.h"
#include "model/entitymanager.h"
#include "model/enums.h"
#include "model/repository/divisionrepository.h"

DivisionModel::DivisionModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DivisionModel::rowCount(const QModelIndex &) const
{
    return m_divisions.size();
}

int DivisionModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant DivisionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        }
    }
    return QVariant();
}

QVariant DivisionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Division *division = m_divisions.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return division->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(division);
    } else if (role == TF::IdRole) {
        return division->id();
    }
    return QVariant();
}

void DivisionModel::fetchDivisions()
{
    beginResetModel();
    m_divisions = m_em->divisionRepository()->loadAll();
    endResetModel();
}
