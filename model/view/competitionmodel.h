#ifndef COMPETITIONMODEL_H
#define COMPETITIONMODEL_H

#include <QAbstractTableModel>

class Event;
class Competition;
class EntityManager;

class CompetitionModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CompetitionModel(Event *event, EntityManager *em, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchCompetitions();

private:
    Event *m_event;
    QList<Competition *> m_competitions;
    EntityManager *m_em;
};

#endif // COMPETITIONMODEL_H
