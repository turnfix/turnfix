#include "disciplineordermodel.h"
#include "model/entity/competitiondiscipline.h"
#include "model/entity/disciplineposition.h"
#include "model/entitymanager.h"
#include "model/repository/disciplinepositionrepository.h"

DisciplineOrderModel::DisciplineOrderModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int DisciplineOrderModel::rowCount(const QModelIndex &) const
{
    return m_rows;
}

int DisciplineOrderModel::columnCount(const QModelIndex &) const
{
    return m_columns;
}

QVariant DisciplineOrderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (m_data.contains(index.row())) {
            QMap<int, DisciplinePosition *> disciplines = m_data.value(index.row());
            if (disciplines.contains(index.column())) {
                auto discipline = disciplines.value(index.column());
                auto name = discipline->competitionDiscipline()->discipline()->name();

                if (discipline->type() == 1) {
                    name += tr(" (K)");
                }

                return name;
            }
        }
    }

    return QVariant();
}

void DisciplineOrderModel::fetchPositions(Competition *competition)
{
    QList<DisciplinePosition *> positions = m_em->disciplinePositionRepository()->loadByCompetition(
        competition);
    beginResetModel();

    // TODO
    //    foreach (auto position, positions) {
    //        if (m_data.at(position->x()).length() == 0) {
    //        }
    //    }

    endResetModel();
}

QModelIndex DisciplineOrderModel::moveUp(const QModelIndex &index)
{
    if (!index.isValid())
        return index;

    int row = index.row();
    int column = index.column();

    if (row == 0)
        return index;

    beginResetModel();

    updateRow(row, column, row - 1);
    updateSize();

    endResetModel();

    return this->index(row - 1, column);
}

QModelIndex DisciplineOrderModel::moveDown(const QModelIndex &index)
{
    if (!index.isValid())
        return index;

    int row = index.row();
    int column = index.column();

    beginResetModel();

    updateRow(row, column, row + 1);
    updateSize();

    endResetModel();

    return this->index(row + 1, column);
}

QModelIndex DisciplineOrderModel::moveLeft(const QModelIndex &index)
{
    if (!index.isValid())
        return index;

    int row = index.row();
    int column = index.column();

    if (column == 0)
        return index;

    beginResetModel();

    updateColumn(row, column, column - 1);
    updateSize();

    endResetModel();

    return this->index(row, column - 1);
}

QModelIndex DisciplineOrderModel::moveRight(const QModelIndex &index)
{
    if (!index.isValid())
        return index;

    int row = index.row();
    int column = index.column();

    beginResetModel();

    updateColumn(row, column, column + 1);
    updateSize();

    endResetModel();

    return this->index(row, column + 1);
}

void DisciplineOrderModel::addDiscipline(CompetitionDiscipline *discipline)
{
    beginResetModel();

    auto pos = new DisciplinePosition();
    pos->setCompetitionDiscipline(discipline);
    pos->setType(0);

    insertDiscipline(pos);

    if (discipline->freeAndCompulsary()) {
        auto pos2 = new DisciplinePosition();
        pos2->setCompetitionDiscipline(discipline);
        pos2->setType(1);

        insertDiscipline(pos2);
    }

    updateSize();
    endResetModel();
}

void DisciplineOrderModel::removeDiscipline(CompetitionDiscipline *discipline)
{
    beginResetModel();
    QList<int> rowKeys = m_data.keys();
    for (auto i : rowKeys) {
        QMap<int, DisciplinePosition *> positions = m_data.value(i);
        QList<int> columnKeys = positions.keys();
        for (auto j : columnKeys) {
            if (discipline == positions.value(j)->competitionDiscipline()) {
                positions.remove(j);
            }
        }
        if (columnKeys.length() == 0) {
            m_data.remove(i);
        } else {
            m_data.insert(i, positions);
        }
    }
    updateSize();
    endResetModel();
}

void DisciplineOrderModel::updateFreeAndCompulsary(Qt::CheckState state,
                                                   CompetitionDiscipline *discipline)
{
    if (state == Qt::Checked) {
        auto pos = new DisciplinePosition();
        pos->setCompetitionDiscipline(discipline);
        pos->setType(1);

        beginResetModel();
        insertDiscipline(pos);
        updateSize();
        endResetModel();

        return;
    }

    beginResetModel();
    QList<int> rowKeys = m_data.keys();
    for (auto i : rowKeys) {
        QMap<int, DisciplinePosition *> positions = m_data.value(i);
        QList<int> columnKeys = positions.keys();
        for (auto j : columnKeys) {
            auto position = positions.value(j);
            auto cd = position->competitionDiscipline();
            if (discipline == cd && position->type() == 1) {
                positions.remove(j);
                disconnect(cd, &CompetitionDiscipline::freeAndCompulsaryChanged, position, nullptr);
            }
        }
        if (columnKeys.length() == 0) {
            m_data.remove(i);
        } else {
            m_data.insert(i, positions);
        }
    }
    updateSize();
    endResetModel();
}

void DisciplineOrderModel::insertDiscipline(DisciplinePosition *position)
{
    auto discipline = position->competitionDiscipline();
    connect(discipline,
            &CompetitionDiscipline::freeAndCompulsaryChanged,
            position,
            [this, discipline](bool freeAndCompulsary) {
                Qt::CheckState state = Qt::Checked;
                if (!freeAndCompulsary) {
                    state = Qt::Unchecked;
                }
                updateFreeAndCompulsary(state, discipline);
            });

    QMap<int, DisciplinePosition *> disciplines;
    if (!m_data.contains(0)) {
        disciplines.insert(0, position);
        m_data.insert(0, disciplines);
    } else {
        disciplines = m_data.value(0);
        QList<int> keys = disciplines.keys();
        auto newMax = 0;
        if (keys.length() > 0) {
            newMax = *std::max_element(keys.begin(), keys.end()) + 1;
        }
        disciplines.insert(newMax, position);
        m_data.insert(0, disciplines);
    }
}

void DisciplineOrderModel::updateRow(int row, int column, int targetRow)
{
    QMap<int, DisciplinePosition *> disciplines1 = m_data.value(row);
    QMap<int, DisciplinePosition *> disciplines2;
    if (m_data.contains(targetRow)) {
        disciplines2 = m_data.value(targetRow);
    }

    DisciplinePosition *selected = disciplines1.value(column);
    if (selected) {
        if (disciplines2.contains(column)) {
            disciplines1.insert(column, disciplines2.value(column));
        } else {
            disciplines1.remove(column);
        }
        disciplines2.insert(column, selected);
    }

    m_data.insert(row, disciplines1);
    m_data.insert(targetRow, disciplines2);
}

void DisciplineOrderModel::updateColumn(int row, int column, int targetColumn)
{
    if (!m_data.contains(row))
        return;

    QMap<int, DisciplinePosition *> disciplines = m_data.value(row);
    DisciplinePosition *selected = disciplines.value(column);
    if (selected) {
        if (disciplines.contains(targetColumn)) {
            disciplines.insert(column, disciplines.value(targetColumn));
        } else {
            disciplines.remove(column);
        }
        disciplines.insert(targetColumn, selected);
    }

    m_data.insert(row, disciplines);
}

void DisciplineOrderModel::updateSize()
{
    int rows = 0;
    int columns = 0;
    QList<int> rowKeys = m_data.keys();
    for (auto i : rowKeys) {
        QList<int> columnKeys = m_data.value(i).keys();
        if (columnKeys.size() == 0)
            continue;
        rows = std::max(rows, i + 1);
        columns = std::max(columns, *std::max_element(columnKeys.begin(), columnKeys.end()) + 1);
    }
    m_rows = rows;
    m_columns = columns;
}
