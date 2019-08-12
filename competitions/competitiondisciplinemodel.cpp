#include "competitiondisciplinemodel.h"
#include "model/entitymanager.h"
#include "model/repository/competitiondisciplinerepository.h"
#include "model/repository/disciplinerepository.h"
#include <QIcon>

CompetitionDisciplineModel::CompetitionDisciplineModel(EntityManager *em, QObject *parent)
    : QAbstractTableModel(parent)
    , m_em(em)
{
}

int CompetitionDisciplineModel::rowCount(const QModelIndex &) const
{
    return m_disciplines.size();
}

int CompetitionDisciplineModel::columnCount(const QModelIndex &) const
{
    return 8;
}

QVariant CompetitionDisciplineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 2:
            return tr("Sport");
        case 3:
            return tr("Disziplin");
        case 4:
            return tr("m/w");
        case 5:
            return tr("Ausschreibungstext");
        case 6:
            return tr("KP");
        case 7:
            return tr("max. Pkt.");
        }
    }
    return QVariant();
}

QVariant CompetitionDisciplineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto discipline = m_disciplines.at(index.row());
    auto competitionDiscipline = m_competitionDisciplines.value(discipline->id());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 2:
            return discipline->sport()->name();
        case 3:
            return discipline->name();
        case 4:
            return discipline->genderText();
        case 5:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->invitationText();
            }
            break;
        case 7:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->maximumScore();
            }
            break;
        }
    } else if (role == Qt::CheckStateRole) {
        switch (index.column()) {
        case 0:
            return competitionDiscipline != nullptr ? Qt::Checked : Qt::Unchecked;
        case 6:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->freeAndCompulsary() ? Qt::Checked : Qt::Unchecked;
            }
            return Qt::Unchecked;
        }
    } else if (role == Qt::DecorationRole) {
        switch (index.column()) {
        case 1:
            return QIcon(discipline->icon());
        }
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 4:
            return Qt::AlignCenter;
        }
    }
    return QVariant();
}

void CompetitionDisciplineModel::fetchDisciplines(Competition *competition, bool women, bool men)
{
    QList<CompetitionDiscipline *> competitionDisciplines = m_em->competitionDisciplineRepository()
                                                                ->fetchByCompetition(competition);

    beginResetModel();
    m_disciplines = m_em->disciplineRepository()->loadByGender(women, men);
    m_competitionDisciplines.clear();
    for (auto competitionDiscipline : competitionDisciplines) {
        m_competitionDisciplines.insert(competitionDiscipline->disciplineId(),
                                        competitionDiscipline);
    }
    endResetModel();
}
