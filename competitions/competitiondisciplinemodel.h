#ifndef COMPETITIONDISCIPLINEMODEL_H
#define COMPETITIONDISCIPLINEMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Competition;
class Discipline;
class CompetitionDiscipline;

class CompetitionDisciplineModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CompetitionDisciplineModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchDisciplines(Competition *competition, bool women, bool men);

private:
    QList<Discipline *> m_disciplines;
    QMap<int, CompetitionDiscipline *> m_competitionDisciplines;
    Competition *m_competition;
    EntityManager *m_em;
};

#endif // COMPETITIONDISCIPLINEMODEL_H
