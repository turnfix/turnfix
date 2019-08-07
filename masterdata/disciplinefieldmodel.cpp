#include "disciplinefieldmodel.h"
#include "model/entity/discipline.h"
#include "model/entity/disciplinefield.h"
#include "model/entitymanager.h"
#include "model/repository/disciplinefieldrepository.h"

DisciplineFieldModel::DisciplineFieldModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineFieldModel::rowCount(const QModelIndex &) const
{
    return m_fields.size();
}

int DisciplineFieldModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant DisciplineFieldModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Feldbezeichnung");
        case 1:
            return tr("EW");
        case 2:
            return tr("AW");
        case 3:
            return tr("Anz.");
        }
    }
    return QVariant();
}


QVariant DisciplineFieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DisciplineField *field = m_fields.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return field->name();
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0:
            return field->name();
        case 1:
            return field->finalScore();
        case 2:
            return field->baseScore();
        case 3:
            return field->enabled();
        }
    } else if (role == Qt::CheckStateRole) {
        switch (index.column()) {
        case 1:
            return field->finalScore() ? Qt::Checked : Qt::Unchecked;
        case 2:
            return field->baseScore() ? Qt::Checked : Qt::Unchecked;
        case 3:
            return field->enabled() ? Qt::Checked : Qt::Unchecked;
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(field);
    } else if (role == TF::IdRole) {
        return field->id();
    }
    return QVariant();
}

bool DisciplineFieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        DisciplineField *field = m_fields.at(index.row());
        switch (index.column()) {
        case 0:
            field->setName(value.toString());
            break;
        case 1:
            field->setFinalScore(value.toBool());
            break;
        case 2:
            field->setBaseScore(value.toBool());
            break;
        case 3:
            field->setEnabled(value.toBool());
            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DisciplineFieldModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() > 0) {
        return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void DisciplineFieldModel::fetchFields(Discipline *discipline)
{
    m_discipline = discipline;
    beginResetModel();
    m_fields = m_em->disciplineFieldRepository()->loadByDiscipline(discipline);
    endResetModel();
}

void DisciplineFieldModel::addField()
{
    auto field = new DisciplineField();
    field->setDiscipline(m_discipline);
    int position = m_fields.size();
    beginInsertRows(QModelIndex(), position, position);
    m_fields.append(field);
    endInsertRows();
}

void DisciplineFieldModel::removeField(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_removedFields.append(m_fields.takeAt(index.row()));
    endRemoveRows();
}

void DisciplineFieldModel::moveUp(const QModelIndex &index)
{
    if (index.row() <= 0) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() - 1);
    m_fields.swapItemsAt(index.row(), index.row() - 1);
    endMoveRows();
}

void DisciplineFieldModel::moveDown(const QModelIndex &index)
{
    if (index.row() >= rowCount()) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() + 2);
    m_fields.swapItemsAt(index.row(), index.row() + 1);
    endMoveRows();
}

void DisciplineFieldModel::persistChanges()
{
    for (int i = 0; i < m_fields.length(); i++) {
        auto field = m_fields.at(i);
        field->setSort(i);
        m_em->disciplineFieldRepository()->persist(field);
    }
    foreach (DisciplineField *field, m_removedFields) {
        m_em->disciplineFieldRepository()->remove(field);
    }
}
