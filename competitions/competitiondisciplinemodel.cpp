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
    return 6;
}

QVariant CompetitionDisciplineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Disziplin");
        case 1:
            return tr("Sport");
        case 2:
            return tr("m/w");
        case 3:
            return tr("Ausschreibungstext");
        case 4:
            return tr("KP");
        case 5:
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
        case 0:
            return discipline->name();
        case 1:
            return discipline->sport()->name();
        case 2:
            return discipline->genderText();
        case 3:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->invitationText();
            }
            break;
        case 5:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->maximumScore();
            }
            break;
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 3:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->invitationText();
            }
            break;
        case 5:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->maximumScore();
            }
            break;
        }
    } else if (role == Qt::CheckStateRole) {
        switch (index.column()) {
        case 0:
            return competitionDiscipline != nullptr ? Qt::Checked : Qt::Unchecked;
        case 4:
            if (competitionDiscipline != nullptr) {
                return competitionDiscipline->freeAndCompulsary() ? Qt::Checked : Qt::Unchecked;
            }
            return Qt::Unchecked;
        }
    } else if (role == Qt::DecorationRole) {
        switch (index.column()) {
        case 0:
            return QIcon(discipline->icon());
        }
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 2:
            return Qt::AlignCenter;
        }
    }
    return QVariant();
}

bool CompetitionDisciplineModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        auto discipline = m_disciplines.at(index.row());
        auto competitionDiscipline = m_competitionDisciplines.value(discipline->id());
        switch (index.column()) {
        case 0:
            if (role == Qt::CheckStateRole) {
                if (value == Qt::Checked) {
                    auto cd = new CompetitionDiscipline();
                    cd->setDiscipline(discipline);
                    cd->setCompetition(m_competition);
                    cd->setFreeAndCompulsary(m_globalFreeAndCompulsary);
                    m_competitionDisciplines.insert(discipline->id(), cd);
                    emit disciplineAdded(cd);
                } else {
                    auto cd = m_competitionDisciplines.value(discipline->id());
                    m_competitionDisciplines.remove(discipline->id());
                    emit disciplineRemoved(cd);
                }

                // Emit data changed for full column
                emit dataChanged(this->index(index.row(), 0),
                                 this->index(index.row(), 5),
                                 QVector<int>() << role << Qt::DisplayRole);

                // move to top
                if (value == Qt::Checked) {
                    int target = m_competitionDisciplines.size() - 1;
                    if (target != index.row()) {
                        beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), target);
                        m_disciplines.swapItemsAt(index.row(), target);
                        endMoveRows();
                    }
                } else {
                    int target = m_competitionDisciplines.size();
                    if (target != index.row()) {
                        beginMoveRows(QModelIndex(),
                                      index.row(),
                                      index.row(),
                                      QModelIndex(),
                                      target + 1);
                        m_disciplines.swapItemsAt(index.row(), target);
                        endMoveRows();
                    }
                }

                return true;
            }
            break;
        case 3:
            if (competitionDiscipline != nullptr) {
                competitionDiscipline->setInvitationText(value.toString());
            }
            break;
        case 4:
            if (competitionDiscipline != nullptr && role == Qt::CheckStateRole) {
                competitionDiscipline->setFreeAndCompulsary(value == Qt::Checked);
            }
            break;
        case 5:
            if (competitionDiscipline != nullptr) {
                competitionDiscipline->setMaximumScore(value.toDouble());
            }
            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags CompetitionDisciplineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    auto discipline = m_disciplines.at(index.row());
    auto competitionDiscipline = m_competitionDisciplines.value(discipline->id());
    if (index.column() == 0) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }
    if (index.column() == 3 || index.column() == 5) {
        if (competitionDiscipline != nullptr) {
            return Qt::ItemIsEditable | Qt::ItemIsEnabled;
        }
    }
    if (index.column() == 4) {
        if (competitionDiscipline != nullptr && !m_globalFreeAndCompulsary) {
            return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
        }
    }

    return Qt::ItemIsEnabled;
}

void CompetitionDisciplineModel::fetchDisciplines(Competition *competition, bool women, bool men)
{
    QList<CompetitionDiscipline *> competitionDisciplines = m_em->competitionDisciplineRepository()
                                                                ->fetchByCompetition(competition);

    beginResetModel();
    m_competition = competition;
    m_disciplines = m_em->disciplineRepository()->loadByGender(women, men);
    m_competitionDisciplines.clear();
    for (auto competitionDiscipline : competitionDisciplines) {
        m_competitionDisciplines.insert(competitionDiscipline->disciplineId(),
                                        competitionDiscipline);
    }

    std::sort(m_disciplines.begin(), m_disciplines.end(), [this](Discipline *a, Discipline *b) {
        int ida = a->id();
        int idb = b->id();
        if (m_competitionDisciplines.contains(ida) && m_competitionDisciplines.contains(idb)) {
            return ida < idb;
        } else if (m_competitionDisciplines.contains(ida)) {
            return true;
        } else if (m_competitionDisciplines.contains(idb)) {
            return false;
        }
        return a->name() < b->name();
    });

    endResetModel();
}

void CompetitionDisciplineModel::reset()
{
    beginResetModel();
    m_competitionDisciplines.clear();
    endResetModel();
}

void CompetitionDisciplineModel::moveUp(const QModelIndex &index)
{
    if (index.row() <= 0) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() - 1);
    m_disciplines.swapItemsAt(index.row(), index.row() - 1);
    endMoveRows();
}

void CompetitionDisciplineModel::moveDown(const QModelIndex &index)
{
    if (index.row() >= rowCount()) {
        return;
    }

    beginMoveRows(QModelIndex(), index.row(), index.row(), QModelIndex(), index.row() + 2);
    m_disciplines.swapItemsAt(index.row(), index.row() + 1);
    endMoveRows();
}

void CompetitionDisciplineModel::updateGlobalFreeAndCompulsary(int state)
{
    if (state == Qt::Unchecked) {
        m_globalFreeAndCompulsary = false;
    } else {
        m_globalFreeAndCompulsary = true;

        beginResetModel();
        QList<int> keys = m_competitionDisciplines.keys();
        foreach (auto key, keys) {
            auto cd = m_competitionDisciplines.value(key);
            cd->setFreeAndCompulsary(true);
            m_competitionDisciplines.insert(key, cd);
        }
        endResetModel();
    }
}
