#ifndef PENALTYMODEL_H
#define PENALTYMODEL_H

#include <QAbstractTableModel>

class EntityManager;
class Penalty;

class PenaltyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PenaltyModel(EntityManager *em, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchPenalties();

private:
    QList<Penalty *> m_penalties;
    EntityManager *m_em;
};

#endif // PENALTYMODEL_H
