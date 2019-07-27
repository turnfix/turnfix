#include "formulamodel.h"
#include "model/entity/formula.h"
#include "model/entitymanager.h"
#include "model/repository/formularepository.h"

FormulaModel::FormulaModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{}

int FormulaModel::rowCount(const QModelIndex &) const
{
    return m_formulas.size();
}

int FormulaModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant FormulaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Formel");
        case 2:
            return tr("Type");
        }
    }
    return QVariant();
}

QVariant FormulaModel::data(const QModelIndex &index, int role) const
{
    Formula *formala = m_formulas.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return formala->name();
        case 1:
            return formala->formula();
        case 2:
            return formala->type();
        }
    } else if (role == Qt::UserRole) {
        return QVariant::fromValue(formala);
    }
    return QVariant();
}

void FormulaModel::fetchFormulas()
{
    beginResetModel();
    m_formulas = m_em->formulaRepository()->loadAll();
    endResetModel();
}
