#include "competitionmodel.h"
#include "model/entity/competition.h"
#include "model/entitymanager.h"
#include "model/repository/competitionrepository.h"

CompetitionModel::CompetitionModel(Event *event, EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_event(event)
    , m_em(em)
{
}

int CompetitionModel::rowCount(const QModelIndex &) const
{
    return m_competitions.size();
}

int CompetitionModel::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant CompetitionModel::headerData(int section, Qt::Orientation orientation, int role) const
{   
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("#");
        case 1:
            return tr("Bezeichnung");
        case 2:
            return tr("Bereich");
        case 3:
            return tr("von Jahr");
        case 4:
            return tr("bis Jahr");
        case 5:
            return tr("Q");
        case 6:
            return tr("Starter");
        }
    }
    return QVariant();
}

QVariant CompetitionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Competition *competition = m_competitions.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return competition->number();
        case 1:
            return competition->name();
        case 2:
            return competition->division()->name();
        case 3:
            return competition->minYearText();
        case 4:
            return competition->maxYearText();
        case 5:
            return competition->qualifiers();
        case 6:
            // TODO get count
            return "-";
        }
    } else if (role == TF::ObjectRole) {
        return QVariant::fromValue(competition);
    } else if (role == TF::IdRole) {
        return competition->id();
    }
    return QVariant();
}

void CompetitionModel::fetchCompetitions()
{
    beginResetModel();
    m_competitions = m_em->competitionRepository()->fetchByEvent(m_event);
    endResetModel();
}
