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
            return tr("Typ");
        }
    }
    return QVariant();
}

QVariant FormulaModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Formula *formula = m_formulas.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return formula->name();
        case 1:
            return formula->formula();
        case 2:
            return formula->type();
        }
    } else if (role == TF::ItemDataRole::ObjectRole) {
        return QVariant::fromValue(formula);
    } else if (role == TF::ItemDataRole::IdRole) {
        return formula->id();
    }
    return QVariant();
}

void FormulaModel::fetchFormulas()
{
    beginResetModel();
    m_formulas = m_em->formulaRepository()->loadAll();
    endResetModel();
}
