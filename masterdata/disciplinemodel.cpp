#include "disciplinemodel.h"
#include "model/entity/discipline.h"
#include "model/entitymanager.h"
#include "model/repository/disciplinerepository.h"

DisciplineModel::DisciplineModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineModel::rowCount(const QModelIndex &) const
{
    return m_disciplines.size();
}

int DisciplineModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DisciplineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Sport");
        }
    }
    return QVariant();
}

QVariant DisciplineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Discipline *discipline = m_disciplines.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return discipline->name();
        case 1:
            return discipline->sport()->name();
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(discipline);
    } else if (role == TF::IdRole) {
        return discipline->id();
    }
    return QVariant();
}

void DisciplineModel::fetchDisciplines()
{
    beginResetModel();
    m_disciplines = m_em->disciplineRepository()->loadAll();
    endResetModel();
}
